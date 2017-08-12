#include "cookbookogl.h"
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>

#include "glutils.h"
#include "scenefire.h"
#include "sceneparticles.h"
#include "sceneparticlesfeedback.h"
#include "sceneparticlesinstanced.h"
#include "scenesmoke.h"
#include "scenewave.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

Scene *scene;
GLFWwindow *window;

string parseCLArgs(int argc, char ** argv);
void printHelpInfo(const char *);

void initializeGL() {
#ifndef __APPLE__
  glDebugMessageCallback(GLUtils::debugCallback, NULL);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
  glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
  GL_DEBUG_SEVERITY_NOTIFICATION, -1 , "Start debugging");
#endif
    glClearColor(0.5f,0.5f,0.5f,1.0f);
    scene->initScene();
}

void mainLoop() {
	while( ! glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) ) {
		GLUtils::checkForOpenGLError(__FILE__,__LINE__);
		scene->update(float(glfwGetTime()));
		scene->render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void resizeGL(int w, int h ) {
    scene->resize(w,h);
}

int main(int argc, char *argv[])
{
	string recipe = parseCLArgs(argc, argv);

	// Initialize GLFW
	if( !glfwInit() ) exit( EXIT_FAILURE );

#ifdef __APPLE__
    // Select OpenGL 4.1
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
#else
    // Select OpenGL 4.3
  	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
  	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
#endif
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Open the window
	string title = "Chapter 9 -- " + recipe;
	window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, title.c_str(), NULL, NULL);
	if( ! window ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	glfwMakeContextCurrent(window);

    // Get framebuffer size
    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);

    // Load the OpenGL functions.
    if(!gladLoadGL()) { exit(-1); }

	GLUtils::dumpGLInfo();

	// Initialization
    resizeGL(fbw,fbh);
    initializeGL();

	// Enter the main loop
	mainLoop();

	// Close window and terminate GLFW
	glfwTerminate();
	// Exit program
	exit( EXIT_SUCCESS );
}

string parseCLArgs(int argc, char ** argv) {

	if( argc < 2 ) {
		printHelpInfo(argv[0]);
		exit(EXIT_FAILURE);
	}

	string recipe = argv[1];

	if( recipe == "fire" ) {
		scene = new SceneFire();
	} else if( recipe == "particles") {
		scene = new SceneParticles();
	} else if( recipe == "particles-feedback") {
		scene = new SceneParticlesFeedback();
	} else if( recipe == "particles-instanced" ) {
		scene = new SceneParticlesInstanced();
	} else if( recipe == "smoke" ) {
		scene = new SceneSmoke();
	} else if( recipe == "wave" ) {
		scene = new SceneWave();
	} else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		printHelpInfo(argv[0]);
		exit(EXIT_FAILURE);
	}

	return recipe;
}

void printHelpInfo(const char * exeFile) {
	printf("Usage: %s recipe-name\n\n", exeFile);
	printf("Recipe names: \n");
	printf("  fire                 : particles simulating fire\n");
	printf("  particles            : a fountain of particles\n");
	printf("  particles-feedback   : a fountain of particles implemented with transform feedback\n");
	printf("  particles-instanced  : a fountain of instanced particles, mmmm.. donuts\n");
	printf("  smoke                : particles simulating smoke\n");
	printf("  wave                 : a plane wave displacement animation\n");
}
