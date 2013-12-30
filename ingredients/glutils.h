#ifndef GLUTILS_H
#define GLUTILS_H

#include "cookbookogl.h"

namespace GLUtils
{
    int checkForOpenGLError(const char *, int);
    
    void dumpGLInfo(bool dumpExtensions = false);
    
    void APIENTRY debugCallback( GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length, const GLchar * msg, const void * param );
}

#endif // GLUTILS_H
