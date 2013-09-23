#version 430

uniform sampler2D ImageTexture;

layout(location=0) out vec4 FragColor;

void main() {
  FragColor = texelFetch(ImageTexture, ivec2(gl_FragCoord.xy), 0); 
}
