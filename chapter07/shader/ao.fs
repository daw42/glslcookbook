#version 400

uniform struct LightInfo {
    vec4 Position;
    vec3 Intensity;
} Light;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

uniform sampler2D AOTex;
uniform sampler2D DiffTex;

vec3 phongModelDiffuse()
{
    vec3 n = Normal;
    if( !gl_FrontFacing ) n = -n;
    vec3 s = normalize(vec3(Light.Position) - Position);
    vec3 v = normalize(-Position.xyz);
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffColor = texture(DiffTex, TexCoord).rgb;
    diffColor = pow( diffColor, vec3(1.8) );   // Gamma un-correct
    diffColor = vec3(1.0f);
    vec3 diffuse = Light.Intensity * diffColor * sDotN;

    return diffuse;
}

void main() {
    vec3 diffuse = phongModelDiffuse();

    vec4 aoFactor = texture(AOTex, TexCoord);

    FragColor = vec4( diffuse, 1.0);

    // Gamma correct
    FragColor = pow( FragColor, vec4(1.0 / 2.2) );
}
