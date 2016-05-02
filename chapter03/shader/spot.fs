#version 410

in vec3 Position;
in vec3 Normal;

struct SpotLightInfo {
    vec4 position;   // Position in eye coords
    vec3 intensity;
    vec3 direction;  // Direction of the spotlight in eye coords.
    float exponent;  // Angular attenuation exponent
    float cutoff;    // Cutoff angle (between 0 and 90)
};
uniform SpotLightInfo Spot;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float Shininess;    // Specular shininess factor

layout( location = 0 ) out vec4 FragColor;

vec3 adsWithSpotlight( )
{
    vec3 n = normalize(Normal);
    vec3 s = normalize( vec3( Spot.position) - Position );
    vec3 spotDir = normalize( Spot.direction);
    float angle = acos( dot(-s, spotDir) );
    float cutoff = radians( clamp( Spot.cutoff, 0.0, 90.0 ) );
    vec3 ambient = Spot.intensity * Ka;

    if( angle < cutoff ) {
        float spotFactor = pow( dot(-s, spotDir), Spot.exponent );
        vec3 v = normalize(vec3(-Position));
        vec3 h = normalize( v + s );

        return
            ambient +
            spotFactor * Spot.intensity * (
              Kd * max( dot(s, n), 0.0 ) +
              Ks * pow( max( dot(h,n), 0.0 ), Shininess )
           );
    } else {
        return ambient;
    }
}

void main() {
    FragColor = vec4(adsWithSpotlight(), 1.0);
}
