#version 430

#define MAX_ITERATIONS 100

layout( local_size_x = 10, local_size_y = 10 ) in;
layout( binding = 0, rgba8) uniform image2D ColorImg;
uniform vec4 CompWindow;
uniform uint Width = 800;
uniform uint Height = 600;

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
  vec4 color = vec4(0.0,0.0,0.0,1.0);
  if( i < MAX_ITERATIONS )
    color.x = (float(i) / 50.0); 

  imageStore(ColorImg, ivec2(gl_GlobalInvocationID.xy), color); 
}
