#version 430

layout(location = 0) in vec4 VPosition;

void main() {
  gl_Position = VPosition;
}
