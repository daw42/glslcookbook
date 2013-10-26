#version 430

layout( local_size_x = 10, local_size_y = 10 ) in;

uniform vec3 Gravity = vec3(0,-10,0);
uniform float ParticleMass = 0.1;
uniform float ParticleInvMass = 1.0 / 0.1;
uniform float SpringK = 2000.0;
uniform float RestLengthHoriz;
uniform float RestLengthVert;
uniform float RestLengthDiag;
uniform float DeltaT = 0.000005;
uniform float DampingConst = 0.1;

layout(std430, binding=0) buffer PosIn {
  vec4 PositionIn[];
};
layout(std430, binding=1) buffer PosOut {
  vec4 PositionOut[];
};
layout(std430, binding=2) buffer VelIn {
  vec4 VelocityIn[];
};
layout(std430, binding=3) buffer VelOut {
  vec4 VelocityOut[];
};

void main() {
  uvec3 nParticles = gl_NumWorkGroups * gl_WorkGroupSize;
  uint idx = gl_GlobalInvocationID.y * nParticles.x + gl_GlobalInvocationID.x;

  vec3 p = vec3(PositionIn[idx]);
  float dist;
  vec3 d;

  // Start with gravitational acceleration and add the spring
  // forces from each neighbor
  vec3 force = Gravity * ParticleMass;

  // Particle above
  if( gl_GlobalInvocationID.y < nParticles.y - 1 ) {
    d = PositionIn[idx + nParticles.x].xyz - p;
    dist = length(d);
    force += normalize(d) * SpringK * (dist - RestLengthVert);
  } 
  // Below
  if( gl_GlobalInvocationID.y > 0 ) {
    d = PositionIn[idx - nParticles.x].xyz - p;
    dist = length(d);
    force += normalize(d) * SpringK * (dist - RestLengthVert);
  } 
  // Left
  if( gl_GlobalInvocationID.x > 0 ) {
    d = PositionIn[idx-1].xyz - p;
    dist = length(d);
    force += normalize(d) * SpringK * (dist - RestLengthHoriz);
  } 
  // Right
  if( gl_GlobalInvocationID.x < nParticles.x - 1 ) {
    d = PositionIn[idx + 1].xyz - p;
    dist = length(d);
    force += normalize(d) * SpringK * (dist - RestLengthHoriz);
  }

  // Diagonals
  // Upper-left
  if( gl_GlobalInvocationID.x > 0 && gl_GlobalInvocationID.y < nParticles.y - 1 ) {
    d = PositionIn[idx + nParticles.x - 1].xyz - p;
    dist = length(d);
    force += normalize(d) * SpringK * (dist - RestLengthDiag);
  }
  // Upper-right
  if( gl_GlobalInvocationID.x < nParticles.x - 1 && gl_GlobalInvocationID.y < nParticles.y - 1) {
    d = PositionIn[idx + nParticles.x + 1].xyz - p;
    dist = length(d);
    force += normalize(d) * SpringK * (dist - RestLengthDiag);
  }
  // lower -left
  if( gl_GlobalInvocationID.x > 0 && gl_GlobalInvocationID.y > 0 ) {
    d = PositionIn[idx - nParticles.x - 1].xyz - p;
    dist = length(d);
    force += normalize(d) * SpringK * (dist - RestLengthDiag);
  }
  // lower-right
  if( gl_GlobalInvocationID.x < nParticles.x - 1 && gl_GlobalInvocationID.y > 0 ) {
    d = PositionIn[idx - nParticles.x + 1].xyz - p;
    dist = length(d);
    force += normalize(d) * SpringK * (dist - RestLengthDiag);
  }

  force += -DampingConst * VelocityIn[idx].xyz;

  // Apply simple Euler integrator
  force = force * ParticleInvMass;
  PositionOut[idx] = vec4(
      p + VelocityIn[idx].xyz * DeltaT + 0.5 * force * DeltaT * DeltaT, 1.0);
  VelocityOut[idx] = vec4( VelocityIn[idx].xyz + force * DeltaT, 0.0);

  // Pin a few of the top verts
  if( gl_GlobalInvocationID.y == nParticles.y - 1 && 
      (gl_GlobalInvocationID.x == 0 || 
       gl_GlobalInvocationID.x == nParticles.x / 4 ||
       gl_GlobalInvocationID.x == nParticles.x * 2 / 4 ||
       gl_GlobalInvocationID.x == nParticles.x * 3 / 4 ||
       gl_GlobalInvocationID.x == nParticles.x - 1)) {
    PositionOut[idx] = PositionIn[idx];
    VelocityOut[idx] = vec4(0,0,0,0);
  }
}

