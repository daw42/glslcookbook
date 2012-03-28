#ifndef GLUTILS_H
#define GLUTILS_H

class GLUtils
{
public:
    GLUtils();

    static int checkForOpenGLError(const char *, int);
    static void dumpGLInfo(bool dumpExtensions = false);
};

#endif // GLUTILS_H
