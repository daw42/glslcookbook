#version 430

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexVelocity;
layout (location = 2) in float VertexStartTime;
layout (location = 3) in vec3 VertexInitialVelocity;

out vec3 Position;   // To transform feedback
out vec3 Velocity;   // To transform feedback
out float StartTime; // To transform feedback
out float Transp;    // To fragment shader

uniform float Time;  // Simulation time
uniform float H;     // Elapsed time between frames
uniform vec3 Accel;  // Particle acceleration
uniform float ParticleLifetime;  // Particle lifespan
uniform float MinParticleSize = 10.0;
uniform float MaxParticleSize = 200.0;

uniform mat4 MVP;

subroutine (RenderPassType)
void update() {

    // Update position & velocity for next frame
    Position = VertexPosition;
    Velocity = VertexVelocity;
    StartTime = VertexStartTime;

    if( Time >= StartTime ) {

        float age = Time - StartTime;

        if( age > ParticleLifetime ) {
            // The particle is past it's lifetime, recycle.
            Position = vec3(0.0);
            Velocity = VertexInitialVelocity;
            StartTime = Time;
        } else {
            // The particle is alive, update.
            Position += Velocity * H;
            Velocity += Accel * H;
        }
    }
}

subroutine (RenderPassType)
void render() {
    float age = Time - VertexStartTime;
    Transp = 0.0;
    if( Time >= VertexStartTime ) {
        float agePct = age/ParticleLifetime;
        Transp = 1.0 - agePct;

        // Intel cards crash when using gl_PointSize
//        gl_PointSize = mix(MinParticleSize,MaxParticleSize,agePct);

    }
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}

void main()
{
    // This will call either render() or update()
    RenderPass();
}
