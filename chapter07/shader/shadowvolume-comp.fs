#version 430

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float Shininess;    // Specular shininess factor

uniform mat4 ProjMatrix;

in vec3 Position;
in vec3 Normal;

layout(binding=0) uniform sampler2D AmbTex;
layout(binding=1) uniform sampler2D DiffSpecTex;

layout( location = 0 ) out vec4 FragColor;

void main() {
  vec4 amb = texelFetch(AmbTex, ivec2(gl_FragCoord), 0);
  vec4 diffSpec = texelFetch(DiffSpecTex, ivec2(gl_FragCoord), 0);

  FragColor = vec4((amb.xyz + diffSpec.xyz), 1);
}
