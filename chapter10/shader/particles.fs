#version 430

in vec3 Position;

uniform vec4 Color;

layout( location = 0 ) out vec4 FragColor;

void main() {
  FragColor = Color;
}
