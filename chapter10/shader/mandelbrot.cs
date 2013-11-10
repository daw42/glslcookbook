#version 430

#define MAX_ITERATIONS 100

layout( local_size_x = 32, local_size_y = 32 ) in;
layout( binding = 0, rgba8) uniform image2D ColorImg;
uniform vec4 CompWindow;
uniform uint Width = 256;
uniform uint Height = 256;

uint mandelbrot( vec2 c ) {
  vec2 z = vec2(0.0,0.0);
  uint i = 0;
  while( i < MAX_ITERATIONS && (z.x*z.x + z.y*z.y) < 4.0 ) {
    z = vec2( z.x*z.x - z.y*z.y + c.x, 2 * z.x *z.y + c.y ); 
    i++;
  }

  return i;
}

void main() {
  float dx = (CompWindow.z - CompWindow.x) / Width; 
  float dy = (CompWindow.w - CompWindow.y) / Height;

  vec2 c = vec2( dx * gl_GlobalInvocationID.x + CompWindow.x,
      dy * gl_GlobalInvocationID.y + CompWindow.y);

  uint i = mandelbrot(c); 
  vec4 color = vec4(0.0,0.5,0.5,1);
  if( i < MAX_ITERATIONS ) {
    if( i < 5 ) color = vec4(float(i)/5.0,0,0,1);
    else if( i < 10 ) color = vec4((float(i)-5.0)/5.0,1,0,1);
    else if( i < 15 ) color = vec4(1,0,(float(i)-10.0)/5.0,1);
    else color = vec4(0,0,1,0);
  }
  else
    color = vec4(0,0,0,1);

  imageStore(ColorImg, ivec2(gl_GlobalInvocationID.xy), color); 
}
