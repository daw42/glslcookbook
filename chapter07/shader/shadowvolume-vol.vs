#version 430

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec2 VertexTexCoord;

out vec3 VPosition;
out vec3 VNormal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjMatrix;

void main()
{ 
    VNormal = NormalMatrix * VertexNormal;
    VPosition = (ModelViewMatrix * vec4(VertexPosition,1.0)).xyz;
    gl_Position = ProjMatrix * ModelViewMatrix * vec4(VertexPosition,1.0);
}
