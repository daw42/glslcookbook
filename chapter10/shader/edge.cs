#version 430

layout (local_size_x = 25, local_size_y = 25) in;

uniform float EdgeThreshold = 0.1;

float luminance( vec3 color ) {
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

shared float localData[gl_WorkGroupSize.x+2][gl_WorkGroupSize.y+2];

layout(binding=0, rgba8) uniform image2D InputImg;
layout(binding=1, rgba8) uniform image2D OutputImg;

void applyFilter()
{
  uvec2 p = gl_LocalInvocationID.xy + uvec2(1,1);

  float sx = localData[p.x-1][p.y-1] + 2 * localData[p.x-1][p.y] + localData[p.x-1][p.y+1]
          - (localData[p.x+1][p.y-1] + 2 * localData[p.x+1][p.y] + localData[p.x+1][p.y+1]);
  float sy = localData[p.x-1][p.y+1] + 2 * localData[p.x  ][p.y+1] + localData[p.x+1][p.y+1]  - 
            (localData[p.x-1][p.y-1] + 2 * localData[p.x  ][p.y-1] + localData[p.x+1][p.y-1]);

  float dist = sx * sx + sy * sy;

  if( dist > EdgeThreshold )
    imageStore(OutputImg, ivec2(gl_GlobalInvocationID.xy), vec4(1.0));
  else
    imageStore(OutputImg, ivec2(gl_GlobalInvocationID.xy), vec4(0,0,0,1));
}

void main()
{
  uvec2 gSize = gl_WorkGroupSize.xy * gl_NumWorkGroups.xy;

  // Copy into local memory
  localData[gl_LocalInvocationID.x+1][gl_LocalInvocationID.y+1] = 
    luminance(imageLoad(InputImg, ivec2(gl_GlobalInvocationID.xy)).rgb);

  // Handle the edges
  // Bottom edge
  if( gl_LocalInvocationID.y == 0 ) {
    if( gl_GlobalInvocationID.y > 0 ) {
      localData[gl_LocalInvocationID.x+1][0] = 
         luminance(imageLoad(InputImg, ivec2(gl_GlobalInvocationID.xy) + ivec2(0,-1)).rgb);
      // Lower left corner
      if( gl_LocalInvocationID.x == 0 )
        if( gl_GlobalInvocationID.x > 0 ) 
          localData[0][0] = 
          luminance(imageLoad(InputImg, ivec2(gl_GlobalInvocationID.xy) + ivec2(-1,-1)).rgb);
        else
          localData[0][0] = 0.0;
      // Lower right corner
      if( gl_LocalInvocationID.x == gl_WorkGroupSize.x - 1 )
        if( gl_GlobalInvocationID.x < gSize.x - 1 )
          localData[gl_WorkGroupSize.x + 1][0] = 
          luminance(imageLoad(InputImg, ivec2(gl_GlobalInvocationID.xy) + ivec2(1,-1)).rgb);
        else
          localData[gl_WorkGroupSize.x+1][0] = 0.0;
    }
    else
      localData[gl_LocalInvocationID.x+1][0] = 0.0;
  }
  // Top edge
  if( gl_LocalInvocationID.y == gl_WorkGroupSize.y - 1 ) {
    if( gl_GlobalInvocationID.y < gSize.y - 1 ) {
      localData[gl_LocalInvocationID.x+1][gl_WorkGroupSize.y+1] = 
         luminance(imageLoad(InputImg, ivec2(gl_GlobalInvocationID.xy) + ivec2(0,1)).rgb);
      // Upper left corner
      if( gl_LocalInvocationID.x == 0 )
        if( gl_GlobalInvocationID.x > 0 ) 
          localData[0][gl_WorkGroupSize.y+1] = 
          luminance(imageLoad(InputImg, ivec2(gl_GlobalInvocationID.xy) + ivec2(-1,1)).rgb);
        else
          localData[0][gl_WorkGroupSize.y+1] = 0.0;
      // Lower right corner
      if( gl_LocalInvocationID.x == gl_WorkGroupSize.x - 1 )
        if( gl_GlobalInvocationID.x < gSize.x - 1 )
          localData[gl_WorkGroupSize.x + 1][gl_WorkGroupSize.y+1] = 
          luminance(imageLoad(InputImg, ivec2(gl_GlobalInvocationID.xy) + ivec2(1,1)).rgb);
        else
          localData[gl_WorkGroupSize.x+1][gl_WorkGroupSize.y+1] = 0.0;
    }
    else
      localData[gl_LocalInvocationID.x+1][gl_WorkGroupSize.y+1] = 0.0;
  }
  // Left edge
  if( gl_LocalInvocationID.x == 0 ) {
    if( gl_GlobalInvocationID.x > 0 ) 
      localData[0][gl_LocalInvocationID.y+1] = 
        luminance(imageLoad(InputImg, ivec2(gl_GlobalInvocationID.xy) + ivec2(-1,0)).rgb);
    else
      localData[0][gl_LocalInvocationID.y+1] = 0.0;
  }
  // Right edge
  if( gl_LocalInvocationID.x == gl_WorkGroupSize.x - 1) {
    if( gl_GlobalInvocationID.x < gSize.x - 1 ) 
      localData[gl_WorkGroupSize.x+1][gl_LocalInvocationID.y+1] = 
        luminance(imageLoad(InputImg, ivec2(gl_GlobalInvocationID.xy) + ivec2(1,0)).rgb);
    else
      localData[gl_WorkGroupSize.x+1][gl_LocalInvocationID.y+1] = 0.0;
  }

  barrier();

  // Apply the filter using local memory
  applyFilter();
}
