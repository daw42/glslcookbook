#version 430

in vec3 Position;
in vec3 Normal;

layout(binding=0) uniform sampler2D DiffSpecTex;
layout( location = 0 ) out vec4 FragColor;

void main() {
  vec4 diffSpec = texelFetch(DiffSpecTex, ivec2(gl_FragCoord), 0);

  FragColor = vec4(diffSpec.xyz, 1);
}
