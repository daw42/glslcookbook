
#include "cookbookogl.h"
#include <GL/glfw.h>

#include "scene.h"
#include "glutils.h"
#include "scenebasic.h"
#include "scenebasic_layout.h"
#include "scenebasic_uniform.h"
#include "scenebasic_uniformblock.h"

#include <string>
using std::string;

Scene *scene;

string parseCLArgs(int argc, char ** argv);
void printHelpInfo(const char *);

void initializeGL() {
    glClearColor(0.2f,0.2f,0.2f,1.0f);
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
	if( !glfwOpenWindow( 640, 640, 8,8,8,8,24,0, GLFW_WINDOW ) ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	string title = "Chapter 01 -- " + recipe;
	glfwSetWindowTitle(title.c_str());

	// Load the OpenGL functions.
	if( ogl_LoadFunctions() == ogl_LOAD_FAILED ) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	GLUtils::dumpGLInfo();

	// Initialization
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

	if( recipe == "basic" ) {
		scene = new SceneBasic();
	} else if( recipe == "basic-layout") {
		scene = new SceneBasic_Layout();
	} else if( recipe == "basic-uniform") {
		scene = new SceneBasic_Uniform();
	} else if( recipe == "basic-uniform-block" ) {
		scene = new SceneBasic_UniformBlock();
	} else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		exit(EXIT_FAILURE);
	}

	return recipe;
}

void printHelpInfo(const char * exeFile) {
	printf("Usage: %s recipe-name\n\n", exeFile);
	printf("Recipe names: \n");
	printf("  basic              : description...\n");
	printf("  basic-layout       : description...\n");
	printf("  basic-uniform      : description...\n");
	printf("  basic-uniform-block: description...\n");
}
