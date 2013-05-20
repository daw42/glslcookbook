#include "cookbookogl.h"
#include <GL/glfw.h>

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
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);

	// Open the window
	if( !glfwOpenWindow( WIN_WIDTH, WIN_HEIGHT, 8,8,8,8,24,0, GLFW_WINDOW ) ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	string title = "Chapter 9 -- " + recipe;
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
	printf("  fire                 : description...\n");
	printf("  particles            : description...\n");
	printf("  particles-feedback   : description...\n");
	printf("  particles-instanced  : description...\n");
	printf("  smoke                : description...\n");
	printf("  wave                 : description...\n");
}
