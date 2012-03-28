#version 400

centroid in vec2 TexCoord;

layout( location = 0 ) out vec4 FragColor;

void main()
{
    vec3 yellow = vec3(1.0,1.0,0.0);
    vec3 blue = vec3(0.0,0.0,1.0);
    vec3 color = blue;
    if( TexCoord.x > 1.0 )
       color = yellow;
    FragColor = vec4( color , 1.0 );
}
