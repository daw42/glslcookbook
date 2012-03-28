#version 400

uniform float LineWidth;
uniform vec4 LineColor;
uniform vec4 QuadColor;

noperspective in vec3 EdgeDistance;

layout ( location = 0 ) out vec4 FragColor;

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

    if( gl_FrontFacing )
        FragColor = mix( QuadColor, LineColor, mixVal );
    else
        FragColor = vec4(1.0,0.0,0.0,1.0);
}
