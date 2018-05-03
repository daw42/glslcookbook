#include "sphere.h"

#include <cstdio>
#include <cmath>

#include <glm/gtc/constants.hpp>

Sphere::Sphere(float radius, GLuint nSlices, GLuint nStacks)
{
    int nVerts = (nSlices+1) * (nStacks + 1);
    int elements = (nSlices * 2 * (nStacks-1) ) * 3;

    // Verts
    std::vector<GLfloat> p(3 * nVerts);
    // Normals
    std::vector<GLfloat> n(3 * nVerts);
    // Tex coords
    std::vector<GLfloat> tex(2 * nVerts);
    // Elements
    std::vector<GLuint> el(elements);

	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / nSlices;
	GLfloat phiFac = glm::pi<float>() / nStacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for( GLuint i = 0; i <= nSlices; i++ ) {
		theta = i * thetaFac;
                s = (GLfloat)i / nSlices;
		for( GLuint j = 0; j <= nStacks; j++ ) {
			phi = j * phiFac;
                        t = (GLfloat)j / nStacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			p[idx] = radius * nx; p[idx+1] = radius * ny; p[idx+2] = radius * nz;
			n[idx] = nx; n[idx+1] = ny; n[idx+2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx+1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for( GLuint i = 0; i < nSlices; i++ ) {
		GLuint stackStart = i * (nStacks + 1);
		GLuint nextStackStart = (i+1) * (nStacks+1);
		for( GLuint j = 0; j < nStacks; j++ ) {
			if( j == 0 ) {
				el[idx] = stackStart;
				el[idx+1] = stackStart + 1;
				el[idx+2] = nextStackStart + 1;
				idx += 3;
			} else if( j == nStacks - 1) {
				el[idx] = stackStart + j;
				el[idx+1] = stackStart + j + 1;
				el[idx+2] = nextStackStart + j;
				idx += 3;
			} else {
				el[idx] = stackStart + j;
				el[idx+1] = stackStart + j + 1;
				el[idx+2] = nextStackStart + j + 1;
				el[idx+3] = nextStackStart + j;
				el[idx+4] = stackStart + j;
				el[idx+5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}

	initBuffers(&el, &p, &n, &tex);
}
