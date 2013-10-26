#version 430

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform vec4 LightPosition;
uniform vec3 LightIntensity;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float Shininess;    // Specular shininess factor

layout( binding = 0 ) uniform sampler2D Tex;

layout( location = 0 ) out vec4 FragColor;

vec3 ads( )
{
    vec3 s = normalize( vec3(LightPosition) - Position );
    vec3 v = normalize(vec3(-Position));
    vec3 r = reflect( -s, Normal );

    return
        LightIntensity * ( Ka +
          Kd * max( dot(s, Normal), 0.0 ) +
          Ks * pow( max( dot(r,v), 0.0 ), Shininess ) );
}

void main() {
  vec4 texColor = texture( Tex, TexCoord );
  FragColor = vec4(ads(), 1.0) * texColor;
}
