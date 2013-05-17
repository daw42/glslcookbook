#include "cookbookogl.h"
#include <GL/glfw.h>

#include "glutils.h"
#include "scene.h"
#include "scenetexture.h"
#include "scenealphatest.h"
#include "scenemultitex.h"
#include "scenenormalmap.h"
#include "sceneprojtex.h"
#include "scenereflectcube.h"
#include "scenerefractcube.h"
#include "scenerendertotex.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

Scene *scene;

string parseCLArgs(int argc, char ** argv);
void printHelpInfo(const char *);

void initializeGL() {
    glClearColor(0.5f,0.5f,0.5f,1.0f);
    scene->initScene();
}

void mainLoop() {
	while( glfwGetWindowParam(GLFW_OPENED) && !glfwGetKey(GLFW_KEY_ESC) ) {
		GLUtils::checkForOpenGLError(__FILE__,__LINE__);
		scene->update(glfwGetTime());
		scene->render();
		glfwSwapBuffers();
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

	// Select OpenGL 3.2 with a forward compatible core profile.
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 4 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 3 );
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

	// Open the window
	if( !glfwOpenWindow( WIN_WIDTH, WIN_HEIGHT, 8,8,8,8,24,0, GLFW_WINDOW ) ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	string title = "Chapter 4 -- " + recipe;
	glfwSetWindowTitle(title.c_str());

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

	if( recipe == "alpha-test" ) {
		scene = new SceneAlphaTest();
	} else if( recipe == "multi-tex") {
		scene = new SceneMultiTex();
	} else if( recipe == "normal-map") {
		scene = new SceneNormalMap();
	} else if( recipe == "proj-tex" ) {
		scene = new SceneProjTex();
	} else if( recipe == "reflect-cube") {
		scene = new SceneReflectCube();
	} else if( recipe == "refract-cube" ) {
		scene = new SceneRefractCube();
	} else if( recipe == "render-to-tex" ) {
		scene = new SceneRenderToTex();
	} else if( recipe == "texture" ) {
		scene = new SceneTexture();
	} else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		exit(EXIT_FAILURE);
	}

	return recipe;
}

void printHelpInfo(const char * exeFile) {
	printf("Usage: %s recipe-name\n\n", exeFile);
	printf("Recipe names: \n");
	printf("  alpha-test    : description...\n");
	printf("  multi-tex     : description...\n");
	printf("  normal-map    : description...\n");
	printf("  proj-tex      : description...\n");
	printf("  reflect-cube  : description...\n");
	printf("  refract-cube  : description...\n");
	printf("  render-to-tex : description...\n");
	printf("  texture       : description...\n");
}
