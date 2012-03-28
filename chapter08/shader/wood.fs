#version 400

uniform sampler2D NoiseTex;

uniform vec4 DarkWoodColor = vec4( 0.8, 0.5, 0.1, 1.0 );
uniform vec4 LightWoodColor = vec4( 1.0, 0.75, 0.25, 1.0 );
uniform mat4 Slice;

in vec2 TexCoord;

layout ( location = 0 ) out vec4 FragColor;

void main()
{
    // Transform the texture coordinates to define the
    // "slice" of the log.
    vec2 tc = TexCoord;
    if( tc.s > 0.5 ) tc.s = 1.0 - tc.s;
    vec4 cyl = Slice * vec4( tc, 0.0, 1.0 );

    // The distance from the log's y axis
    float dist = length(cyl.xz);

    // Perturb the distance using the noise texture.
    vec4 noise = texture(NoiseTex, tc);
    dist += noise.b * 2.5;

    // Determine the color as a mixture of the light and
    // dark wood colors
    float t = 1.0 - abs( fract( dist ) * 2.0 - 1.0 );
    t = smoothstep( 0.2, 0.5, t );
    vec4 color = mix( DarkWoodColor, LightWoodColor, t );

    FragColor = vec4( color.rgb , 1.0 );
}
