#include "glutils.h"

#include "cookbookogl.h"

#include <cstdio>

GLUtils::GLUtils() {}

int GLUtils::checkForOpenGLError(const char * file, int line) {
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
		const char * message = "";
		switch( glErr )
		{
		case GL_INVALID_ENUM:
			message = "Invalid enum";
			break;
		case GL_INVALID_VALUE:
			message = "Invalid value";
			break;
		case GL_INVALID_OPERATION:
			message = "Invalid operation";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			message = "Invalid framebuffer operation";
			break;
		case GL_OUT_OF_MEMORY:
			message = "Out of memory";
			break;
		default:
			message = "Unknown error";
		}

        printf("glError in file %s @ line %d: %s\n", file, line, message);
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

void GLUtils::dumpGLInfo(bool dumpExtensions) {
    const GLubyte *renderer = glGetString( GL_RENDERER );
    const GLubyte *vendor = glGetString( GL_VENDOR );
    const GLubyte *version = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
	
	printf("-------------------------------------------------------------\n");
    printf("GL Vendor    : %s\n", vendor);
    printf("GL Renderer  : %s\n", renderer);
    printf("GL Version   : %s\n", version);
    printf("GL Version   : %d.%d\n", major, minor);
    printf("GLSL Version : %s\n", glslVersion);
    printf("-------------------------------------------------------------\n");

    if( dumpExtensions ) {
        GLint nExtensions;
        glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
        for( int i = 0; i < nExtensions; i++ ) {
            printf("%s\n", glGetStringi(GL_EXTENSIONS, i));
        }
    }
}
