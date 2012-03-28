#version 400

layout (location = 0) in vec3 VertexPosition;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    gl_Position = ModelViewMatrix * vec4(VertexPosition,1.0);
}
