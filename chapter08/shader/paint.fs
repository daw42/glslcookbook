#version 400

uniform struct LightInfo {
    vec4 Position;
    vec3 Intensity;
} Light;

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
} Material;

uniform sampler2D NoiseTex;

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 PaintColor = vec3(1.0);
uniform float Threshold = 0.62;

layout ( location = 0 ) out vec4 FragColor;

vec3 phongModel(vec3 kd) {
    vec3 n = Normal;
    if( !gl_FrontFacing ) n = -n;
    vec3 s = normalize(Light.Position.xyz - Position.xyz);
    vec3 v = normalize(-Position.xyz);
    vec3 r = reflect( -s, n );
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffuse = Light.Intensity * kd * sDotN;
    vec3 spec = vec3(0.0);
    if( sDotN > 0.0 )
        spec = Light.Intensity * Material.Ks *
            pow( max( dot(r,v), 0.0 ), Material.Shininess );

    return diffuse + spec;
}

void main()
{
    vec4 noise = texture( NoiseTex, TexCoord );
    vec3 color = Material.Kd;
    if( noise.a > Threshold ) color = PaintColor;

    FragColor = vec4( phongModel(color) , 1.0 );
}
