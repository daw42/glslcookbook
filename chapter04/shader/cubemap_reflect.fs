#version 430

in vec3 ReflectDir;

uniform samplerCube CubeMapTex;

uniform bool DrawSkyBox;
uniform float ReflectFactor;
uniform vec4 MaterialColor;

layout( location = 0 ) out vec4 FragColor;

void main() {
    // Access the cube map texture
    vec4 cubeMapColor = texture(CubeMapTex, ReflectDir);

    if( DrawSkyBox )
        FragColor = cubeMapColor;
    else
        FragColor = mix( MaterialColor, cubeMapColor, ReflectFactor);
}
