#include "cookbookogl.h"
#include <GLFW/glfw3.h>

#include "glutils.h"
#include "scenebezcurve.h"
#include "scenepointsprite.h"
#include "scenequadtess.h"
#include "sceneshadewire.h"
#include "scenesilhouette.h"
#include "scenetessteapot.h"
#include "scenetessteapotdepth.h"

#include <cstdio>
#include <cstdlib>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

Scene *scene;
GLFWwindow *window;

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
	string title = "Chapter 6 -- " + recipe;
	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, title.c_str(), NULL, NULL );
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
	resizeGL(fbw, fbh);
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

	if( recipe == "bez-curve" ) {
		scene = new SceneBezCurve();
	} else if( recipe == "point-sprite") {
		scene = new ScenePointSprite();
	} else if( recipe == "quad-tess") {
		scene = new SceneQuadTess();
	} else if( recipe == "shade-wire" ) {
		scene = new SceneShadeWire();
	} else if( recipe == "silhouette") {
		scene = new SceneSilhouette();
	} else if( recipe == "tess-teapot" ) {
		scene = new SceneTessTeapot();
	} else if( recipe == "tess-teapot-depth" ) {
		scene = new SceneTessTeapotDepth();
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
	printf("  bez-curve         : 2D Bezier curve with tessellation shader\n");
	printf("  point-sprite      : Point sprites with the geometry shader\n");
	printf("  quad-tess         : Demonstrates how quad tessellation works\n");
	printf("  shade-wire        : Uses the geometry shader to draw a mesh over a shaded object\n");
	printf("  silhouette        : Uses the geometry shader to draw silhouette edges\n");
	printf("  tess-teapot       : Uses tessellation to draw a teapot\n");
	printf("  tess-teapot-depth : Varies the amount of tessellation with depth\n");
}
