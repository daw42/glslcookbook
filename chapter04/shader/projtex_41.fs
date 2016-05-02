#version 410

in vec3 EyeNormal;       // Normal in eye coordinates
in vec4 EyePosition;     // Position in eye coordinates
in vec4 ProjTexCoord;

uniform sampler2D ProjectorTex;

struct MaterialInfo {
    vec3 Kd;
    vec3 Ks;
    vec3 Ka;
    float Shininess;
};
uniform MaterialInfo Material;

struct LightInfo {
    vec3 Intensity;
    vec4 Position;
};
uniform LightInfo Light;

layout( location = 0 ) out vec4 FragColor;

vec3 phongModel( vec3 pos, vec3 norm ) {
    vec3 s = normalize(vec3(Light.Position) - pos);
    vec3 v = normalize(-pos.xyz);
    vec3 r = reflect( -s, norm );
    vec3 ambient = Light.Intensity * Material.Ka;
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = Light.Intensity * Material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Intensity * Material.Ks *
               pow( max( dot(r,v), 0.0 ), Material.Shininess );

    return ambient + diffuse + spec;
}

void main() {
    vec3 color = phongModel(vec3(EyePosition), normalize(EyeNormal));

    vec4 projTexColor = vec4(0.0);
    if( ProjTexCoord.z > 0.0 )
        projTexColor = textureProj( ProjectorTex, ProjTexCoord );

    FragColor = vec4(color,1.0) + projTexColor * 0.5;
}
