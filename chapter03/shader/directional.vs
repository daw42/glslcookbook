#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 Color;

// Normalized direction towards light source in eye coords.
uniform vec4 LightPosition;
uniform vec3 LightIntensity;

uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;            // Ambient reflectivity
uniform vec3 Ks;            // Specular reflectivity
uniform float Shininess;    // Specular shininess factor

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

vec3 ads( vec4 position, vec3 norm )
{
    vec3 s;
    if( LightPosition.w == 0.0 )
      s = normalize(vec3(LightPosition));
    else
      s = normalize( vec3(LightPosition - position) );

    vec3 v = normalize(vec3(-position));
    vec3 r = reflect( -s, norm );

    return
        LightIntensity * ( Ka +
           Kd * max( dot(s, norm), 0.0 ) +
           Ks * pow( max( dot(r,v), 0.0 ), Shininess ) );
}

void main()
{
    vec3 eyeNorm = normalize( NormalMatrix * VertexNormal);
    vec4 eyePosition = ModelViewMatrix * vec4(VertexPosition,1.0);;

    // Evaluate the lighting equation
    Color = ads( eyePosition, eyeNorm );

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
