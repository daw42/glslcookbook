#include "cookbookogl.h"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

#include "glutils.h"
#include "scenemandelbrot.h"
#include "scenecloth.h"
#include "sceneparticles.h"
#include "sceneedge.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#include <sstream>
using std::stringstream;

Scene *scene;
GLFWwindow *window;
string title;

string parseCLArgs(int argc, char ** argv);
void printHelpInfo(const char *);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    if( scene )
      scene->animate( ! (scene->animating()) );
}

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
  const int samples = 50;
  float time[samples];
  int index = 0;

  while( ! glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) ) {
    GLUtils::checkForOpenGLError(__FILE__,__LINE__);
    scene->update(float(glfwGetTime()));
    scene->render();
    glfwSwapBuffers(window);
    glfwPollEvents();

    // Update FPS
    time[index] = float(glfwGetTime());
    index = (index + 1) % samples;

    if( index == 0 ) {
      float sum = 0.0f;
      for( int i = 0; i < samples-1 ; i++ )
        sum += time[i + 1] - time[i];
      float fps = samples / sum;

      stringstream strm;
      strm << title;
      strm.precision(4);
      strm << " (fps: " << fps << ")";
      glfwSetWindowTitle(window, strm.str().c_str());
    }
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
	title = "Chapter 10 -- " + recipe;
	window = glfwCreateWindow( WIN_WIDTH, WIN_HEIGHT, title.c_str(), NULL, NULL);
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
	initializeGL();
	resizeGL(fbw, fbh);

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

	if( recipe == "mandelbrot" ) {
        scene = new SceneMandelbrot();
        } else if( recipe == "cloth" ) {
          scene = new SceneCloth();
        } else if( recipe == "particles" ) {
          scene = new SceneParticles();
        } else if( recipe == "edge" ) {
          scene = new SceneEdge();
	} else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		printHelpInfo(argv[0]);
		exit(EXIT_FAILURE);
	}

	return recipe;
}

void printHelpInfo(const char * exeFile) {
    printf("Note: Chapter 10 examples are not supported on MacOS.\n");
	printf("Usage: %s recipe-name\n\n", exeFile);
	printf("Recipe names: \n");
        printf("  particles            : Simple particle simulation\n");
	printf("  mandelbrot           : Mandelbrot set with compute shader\n");
        printf("  cloth                : Cloth simulation with compute shader\n");
        printf("  edge                 : Edge detection filter using compute shader\n");
}
