#include "noisetex.h"

#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

int NoiseTex::generate2DTex(float baseFreq, float persistence, int w, int h) {

    int width = w;
    int height = h;

    printf("Generating noise texture...");

    GLubyte *data = new GLubyte[ width * height * 4 ];

    double xRange = 1.0;
    double yRange = 1.0;
    double xFactor = xRange / width;
    double yFactor = yRange / height;

	for( int i = 0; i < width; i++ ) {
		for( int j = 0 ; j < height; j++ ) {
			float x = xFactor * i;
			float y = yFactor * j;
			float sum = 0.0f;
			float freq = baseFreq;
			float persist = persistence;
			for( int oct = 0; oct < 4; oct++ ) {
				glm::vec2 p(x * freq, y * freq);

				float val = glm::perlin(p) * persist;

				sum += val;

				float result = sum + 0.5;

				// Clamp strictly between 0 and 1
				result = result > 1.0f ? 1.0f : result;
				result = result < 0.0f ? 0.0f : result;

				// Store in texture
				data[((j * width + i) * 4) + oct] = (GLubyte) ( result * 255.0f );
				freq *= 2.0f;
				persist *= persistence;
			}
		}
	}

    GLuint texID;
    glGenTextures(1, &texID);

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete [] data;

    printf("done.\n");
    return texID;
}
