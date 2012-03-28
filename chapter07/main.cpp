#include "cookbookogl.h"
#include <glload/gll.hpp>
#include <GL/freeglut.h>

#include "glutils.h"
#include "sceneao.h"
#include "scenejitter.h"
#include "scenepcf.h"
#include "sceneshadowmap.h"

Scene *scene;

void initializeGL() {

    //////////////// PLUG IN SCENE HERE /////////////////
    scene = new SceneShadowMap();
    ////////////////////////////////////////////////////

    GLUtils::dumpGLInfo();

    glClearColor(0.5f,0.5f,0.5f,1.0f);
    scene->initScene();
}

void paintGL() {
    GLUtils::checkForOpenGLError(__FILE__,__LINE__);
    scene->render();
    glutSwapBuffers();
}

void resizeGL(int w, int h ) {
    scene->resize(w,h);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE );
	glutInitContextVersion (4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
		
	glutInitWindowSize (800, 600);
	glutInitWindowPosition (300, 200);
	glutCreateWindow (argv[0]);

	glload::LoadFunctions();

	initializeGL();
	glutDisplayFunc(paintGL);
	glutReshapeFunc(resizeGL);

	glutMainLoop();
	return 0;
}

