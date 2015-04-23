#include "noisetex.h"

#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

int NoiseTex::storeTex( GLubyte * data, int w, int h ) {
  GLuint texID;
  glGenTextures(1, &texID);

  glBindTexture(GL_TEXTURE_2D, texID);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);
  glTexSubImage2D(GL_TEXTURE_2D,0,0,0,w,h,GL_RGBA,GL_UNSIGNED_BYTE,data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  return texID;
}

int NoiseTex::generate2DTex(float baseFreq, float persistence, int w, int h, bool periodic) {

  int width = w;
  int height = h;

  printf("Generating noise texture...");

  GLubyte *data = new GLubyte[ width * height * 4 ];

  float xFactor = 1.0f / (width - 1);
  float yFactor = 1.0f / (height - 1);

  for( int row = 0; row < height; row++ ) {
    for( int col = 0 ; col < width; col++ ) {
      float x = xFactor * col;
      float y = yFactor * row;
      float sum = 0.0f;
      float freq = baseFreq;
      float persist = persistence;
      for( int oct = 0; oct < 4; oct++ ) {
        glm::vec2 p(x * freq, y * freq);

        float val = 0.0f;
        if (periodic) {
          val = glm::perlin(p, glm::vec2(freq)) * persist;
        } else {
          val = glm::perlin(p) * persist;
        }

        sum += val;

        float result = (sum + 1.0f) / 2.0f;

        // Clamp strictly between 0 and 1
        result = result > 1.0f ? 1.0f : result;
        result = result < 0.0f ? 0.0f : result;

        // Store in texture
        data[((row * width + col) * 4) + oct] = (GLubyte) ( result * 255.0f );
        freq *= 2.0f;
        persist *= persistence;
      }
    }
  }

  int texID = NoiseTex::storeTex(data, width, height);
  delete [] data;

  printf("done.\n");
  return texID;
}

int NoiseTex::generatePeriodic2DTex(float baseFreq, float persist, int w, int h) {
  return NoiseTex::generate2DTex(baseFreq, persist, w, h, true);
}
