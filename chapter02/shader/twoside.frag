#version 410

in vec3 FrontColor;
in vec3 BackColor;

layout( location = 0 ) out vec4 FragColor;

void main() {

    if( gl_FrontFacing ) {
        FragColor = vec4(FrontColor, 1.0);
    } else {
        //FragColor = mix( vec4(BackColor, 1.0), vec4(1.0,0.0,0.0,1.0), 0.7 );
        FragColor = vec4(BackColor, 1.0);
    }
}
