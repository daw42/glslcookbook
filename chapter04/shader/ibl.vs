#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 ViewDir;   // View direction in world coordinates
out vec3 Normal;    // Normal in world coordinates

uniform bool DrawSkyBox;

uniform vec3 WorldCameraPosition;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    vec3 worldPos = vec3( ModelMatrix * vec4(VertexPosition,1.0) );
    Normal = normalize( vec3(ModelMatrix * vec4(VertexNormal, 0.0)) );
    ViewDir = normalize( WorldCameraPosition - worldPos );

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
