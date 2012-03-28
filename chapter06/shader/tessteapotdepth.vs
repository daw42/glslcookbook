#version 400

layout (location = 0 ) in vec3 VertexPosition;

void main()
{
    gl_Position = vec4(VertexPosition, 1.0);
}
