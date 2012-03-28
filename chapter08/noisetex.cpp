#include "noisetex.h"

#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

int NoiseTex::generate2DTex(bool seamless, float baseFreq, int w, int h) {

    int width = w;
    int height = h;

    // Base frequency for octave 1.
    //perlinNoise.SetFrequency(baseFreq);

    printf("Generating noise texture...");

    GLubyte *data = new GLubyte[ width * height * 4 ];

    double xRange = 1.0;
    double yRange = 1.0;
    double xFactor = xRange / width;
    double yFactor = yRange / height;
    for( int oct = 0; oct < 4; oct++ ) {
		
		// Do something about octaves here

        for( int i = 0; i < width; i++ ) {
            for( int j = 0 ; j < height; j++ ) {

				glm::vec2 p(xFactor * i * 16.0f, yFactor * j * 16.0f);

                float val = 0.0f;
                if( !seamless ) {
                    val = glm::perlin(p);
                } else {
                    val = glm::perlin(p, glm::vec2(16.0f));
                }
				
                // Scale to roughly between 0 and 1
                //val = (val + 1.0f) * 0.5f;

                // Clamp strictly between 0 and 1
                //val = val > 1.0f ? 1.0f : val;
                //val = val < 0.0f ? 0.0f : val;

                // Store in texture
                data[((j * width + i) * 4) + oct] = (GLubyte) ( val * 255.0f );
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
