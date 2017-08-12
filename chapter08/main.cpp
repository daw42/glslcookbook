#include "cookbookogl.h"
#include <GLFW/glfw3.h>

#include "glutils.h"
#include "scenedecay.h"
#include "scenenightvision.h"
#include "scenepaint.h"
#include "scenesky.h"
#include "scenewood.h"
#include "scenenoise.h"

#include <cstdio>
#include <cstdlib>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

Scene *scene;
GLFWwindow * window;

string parseCLArgs(int argc, char ** argv);
void printHelpInfo(const char *);

void initializeGL() {
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

	// Open the window
	string title = "Chapter 8 -- " + recipe;
	window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, title.c_str(), NULL, NULL);
	if( !window ) {
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

	if( recipe == "decay" ) {
		scene = new SceneDecay();
	} else if( recipe == "noise" ) {
          scene = new SceneNoise();
        } else if( recipe == "night-vision") {
		scene = new SceneNightVision();
	} else if( recipe == "paint") {
		scene = new ScenePaint();
	} else if( recipe == "sky" ) {
		scene = new SceneSky();
	} else if( recipe == "wood" ) {
		scene = new SceneWood();
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
        printf("  noise        : just display the raw noise texture\n");
	printf("  decay        : decay of a teapot\n");
	printf("  night-vision : night vision goggles\n");
	printf("  paint        : paint spatters on a teapot\n");
	printf("  sky          : clouds and sky\n");
	printf("  wood         : wood \n");

}
