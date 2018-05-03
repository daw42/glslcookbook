#include "scene.h"
#include "scenerunner.h"
#include "scenebasic.h"
#include "scenebasic_attrib.h"
#include "scenebasic_debug.h"
#include "scenebasic_uniform.h"
#include "scenebasic_uniformblock.h"

#include <memory>

static std::map< std::string, std::string > sceneData = {
		{"basic", "Basic scene."},
		{"basic-attrib", "Prints active attributes."},
		{"basic-debug", "Display debug messages."},
		{"basic-uniform", "Basic scene with a uniform variable."},
		{"basic-uniform-block", "Scene with a uniform block variable."}
};

int main(int argc, char *argv[])
{
	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneData);

	SceneRunner runner("Chapter 1 - " + recipe, 500, 500);

	std::unique_ptr<Scene> scene;
	if( recipe == "basic" ) {
		scene = std::unique_ptr<Scene>(new SceneBasic());
	} else if( recipe == "basic-attrib") {
		scene = std::unique_ptr<Scene>(new SceneBasic_Attrib());
	} else if( recipe == "basic-uniform") {
		scene = std::unique_ptr<Scene>(new SceneBasic_Uniform());
	} else if( recipe == "basic-uniform-block" ) {
		scene = std::unique_ptr<Scene>(new SceneBasic_UniformBlock());
	} else if( recipe == "basic-debug" ) {
		scene = std::unique_ptr<Scene>(new SceneBasic_Debug());
	}

	return runner.run(*scene);
}


