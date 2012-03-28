#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

out vec3 Color;

//uniform struct {
//  mat4 RotationMatrix;
//  mat4 ViewMatrix; } MyMats;

uniform mat4 RotationMatrix;

//uniform mat4 Mats[2];

void main()
{
    Color = VertexColor;
    gl_Position = RotationMatrix * vec4(VertexPosition,1.0);
}
