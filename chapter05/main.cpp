#include "cookbookogl.h"
#include <GLFW/glfw3.h>

#include "glutils.h"
#include "scenebloom.h"
#include "sceneblur.h"
#include "scenedeferred.h"
#include "sceneedge.h"
#include "scenegamma.h"
#include "scenemsaa.h"
#include "scenetonemap.h"
#include "scenehdrbloom.h"

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
		scene->update(glfwGetTime());
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

	// Select OpenGL 4.3 with a forward compatible core profile.
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	// Open the window
	string title = "Chapter 5 -- " + recipe;
	window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, title.c_str(), NULL, NULL );
	if( ! window ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	glfwMakeContextCurrent(window);

	// Load the OpenGL functions.
	if( ogl_LoadFunctions() == ogl_LOAD_FAILED ) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	GLUtils::dumpGLInfo();

	// Initialization
	initializeGL();
	resizeGL(WIN_WIDTH,WIN_HEIGHT);

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

	if( recipe == "bloom" ) {
		scene = new SceneBloom();
	} else if( recipe == "blur") {
		scene = new SceneBlur();
	} else if( recipe == "deferred") {
		scene = new SceneDeferred();
	} else if( recipe == "edge" ) {
		scene = new SceneEdge();
	} else if( recipe == "gamma") {
		scene = new SceneGamma();
	} else if( recipe == "msaa" ) {
		scene = new SceneMsaa();
        } else if( recipe == "tone-map" ) {
            scene = new SceneToneMap();
        } else if( recipe == "hdr-bloom" ) {
          scene = new SceneHdrBloom();
	} else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		exit(EXIT_FAILURE);
	}

	return recipe;
}

void printHelpInfo(const char * exeFile) {
	printf("Usage: %s recipe-name\n\n", exeFile);
	printf("Recipe names: \n");
	printf("  bloom    : description...\n");
	printf("  blur     : description...\n");
	printf("  deferred : description...\n");
	printf("  edge     : description...\n");
	printf("  gamma    : description...\n");
	printf("  msaa     : description...\n");
        printf("  tone-map : tone mapping example.\n");
        printf("  hdr-bloom: bloom example with HDR tone mapping.\n");
}

