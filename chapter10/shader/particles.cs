#version 440

layout( local_size_x = 1000 ) in;

uniform float Gravity1 = 1000.0;
uniform vec3 BlackHolePos1 = vec3(5,0,0);

uniform float Gravity2 = 1000.0;
uniform vec3 BlackHolePos2 = vec3(-5,0,0);

uniform float ParticleMass = 0.1;
uniform float ParticleInvMass = 1.0 / 0.1;
uniform float DeltaT = 0.0005;
uniform float MaxDist = 45.0;

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
  uint idx = gl_GlobalInvocationID.x;

  vec3 p = PositionIn[idx].xyz;

  // Force from black hole #1
  vec3 d = BlackHolePos1 - p;
  float dist = length(d);
  vec3 force = (Gravity1 / dist) * normalize(d);
  
  // Force from black hole #2
  d = BlackHolePos2 - p;
  dist = length(d);
  force += (Gravity2 / dist) * normalize(d);

  // Reset particles that get too far from the attractors
  if( dist > MaxDist ) {
    PositionOut[idx] = vec4(0,0,0,1);
  } else {
    // Apply simple Euler integrator
    vec3 a = force * ParticleInvMass;
    PositionOut[idx] = vec4(
        p + VelocityIn[idx].xyz * DeltaT + 0.5 * a * DeltaT * DeltaT, 1.0);
    VelocityOut[idx] = vec4( VelocityIn[idx].xyz + a * DeltaT, 0.0);
  }
}

