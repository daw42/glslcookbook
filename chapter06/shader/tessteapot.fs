#version 400

uniform float LineWidth;
uniform vec4 LineColor;
uniform vec4 LightPosition;
uniform vec3 LightIntensity;
uniform vec3 Kd;

noperspective in vec3 EdgeDistance;
in vec3 Normal;
in vec4 Position;

layout ( location = 0 ) out vec4 FragColor;

vec3 diffuseModel( vec3 pos, vec3 norm )
{
    vec3 s = normalize(vec3(LightPosition) - pos);
    float sDotN = max( dot(s,norm), 0.0 );
    vec3 diffuse = LightIntensity * Kd * sDotN;

    return diffuse;
}

float edgeMix()
{
    // Find the smallest distance
    float d = min( min( EdgeDistance.x, EdgeDistance.y ), EdgeDistance.z );

    if( d < LineWidth - 1 ) {
        return 1.0;
    } else if( d > LineWidth + 1 ) {
        return 0.0;
    } else {
        float x = d - (LineWidth - 1);
        return exp2(-2.0 * (x*x));
    }
}

void main()
{
    float mixVal = edgeMix();
    vec4 color = vec4( diffuseModel( Position.xyz, Normal ), 1.0);
    color = pow( color, vec4(1.0/2.2) );
    FragColor = mix( color, LineColor, mixVal );
}
