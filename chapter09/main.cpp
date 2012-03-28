#include "cookbookogl.h"
#include <glload/gll.hpp>
#include <GL/freeglut.h>

#include "glutils.h"
#include "animtimer.h"
#include "scenefire.h"
#include "sceneparticles.h"
#include "sceneparticlesfeedback.h"
#include "sceneparticlesinstanced.h"
#include "scenesmoke.h"
#include "scenewave.h"

Scene *scene;
AnimTimer animTimer;

void initializeGL() {

    //////////////// PLUG IN SCENE HERE /////////////////
    scene = new SceneWave();
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

void updateGL() {
	scene->update(animTimer.curTime());
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL );
	glutInitContextVersion (4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
		
	glutInitWindowSize (800, 600);
	glutInitWindowPosition (300, 200);
	glutCreateWindow (argv[0]);

	glload::LoadFunctions();

	initializeGL();
	glutDisplayFunc(paintGL);
	glutReshapeFunc(resizeGL);
	glutIdleFunc(updateGL);

	glutMainLoop();
	return 0;
}

