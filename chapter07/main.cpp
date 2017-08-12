#include "cookbookogl.h"
#include <GLFW/glfw3.h>

#include "glutils.h"
#include "sceneao.h"
#include "scenejitter.h"
#include "scenepcf.h"
#include "sceneshadowmap.h"
#include "sceneshadowvolume.h"

#include <cstdio>
#include <cstdlib>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

Scene *scene;
GLFWwindow *window;

string parseCLArgs(int argc, char ** argv);
void printHelpInfo(const char *);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    if( scene )
      scene->animate( ! (scene->animating()) );
}

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
	string title = "Chapter 7 -- " + recipe;
	window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, title.c_str(), NULL, NULL );
	if( ! window ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	glfwMakeContextCurrent(window);

        glfwSetKeyCallback(window, key_callback);

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

	if( recipe == "ao" ) {
		scene = new SceneAo();
	} else if( recipe == "jitter") {
		scene = new SceneJitter();
	} else if( recipe == "pcf") {
		scene = new ScenePcf();
	} else if( recipe == "shadow-map" ) {
		scene = new SceneShadowMap();
        } else if( recipe == "shadow-volume" ) {
          scene = new SceneShadowVolume();
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
	printf("  ao          : Ambient occlusion from a texture\n");
	printf("  jitter      : Blur shadow map edges using a random jitter\n");
	printf("  pcf         : Blur shadow map edges using percentage-closer-filtering\n");
	printf("  shadow-map  : Simple shadow map\n");
        printf("  shadow-volume: Shadow Volumes using geometry shader.\n");
}
