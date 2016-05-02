#version 410

in vec3 Position;
in vec3 Normal;

struct LightInfo {
    vec4 position;
    vec3 intensity;
};
uniform LightInfo Light;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity

const int levels = 3;
const float scaleFactor = 1.0 / levels;

layout( location = 0 ) out vec4 FragColor;

vec3 toonShade( )
{
    vec3 n = normalize(Normal);
    vec3 s = normalize( Light.position.xyz - Position.xyz );
    vec3 ambient = Ka;
    float cosine = dot( s, n );
    vec3 diffuse = Kd * floor( cosine * levels ) * scaleFactor;

    return Light.intensity * (ambient + diffuse);
}

void main() {
    FragColor = vec4(toonShade(), 1.0);
}
