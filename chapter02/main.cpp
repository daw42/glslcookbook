#include "scene.h"
#include "scenerunner.h"
#include "sceneads.h"
#include "scenediffuse.h"
#include "scenediscard.h"
#include "sceneflat.h"
#include "scenesubroutine.h"
#include "scenetwoside.h"

#include <map>

static std::map< std::string, std::string > sceneData = {
		{"ads",        "Ambient, diffuse, and specular shading."},
		{"diffuse",    "Diffuse shading only"},
		{"discard",    "Example of discarding fragments"},
		{"flat",       "Flat shading"},
		{"subroutine", "Using a shader subroutine"},
		{"two-side",   "Two-sided lighting"}
};

int main( int argc, char ** argv) {

	std::string recipeName = SceneRunner::parseCLArgs(argc, argv, sceneData);
    SceneRunner runner("Chapter 02 - " + recipeName);

	std::unique_ptr<Scene> scene;
	if(recipeName == "ads") {
		scene = std::unique_ptr<Scene>(new SceneADS());
	}
	else if( recipeName == "diffuse") {
		scene = std::unique_ptr<Scene>(new SceneDiffuse());
	}
	else if( recipeName == "discard" ) {
		scene = std::unique_ptr<Scene>(new SceneDiscard());
	}
	else if( recipeName == "flat" ) {
		scene = std::unique_ptr<Scene>(new SceneFlat());
	}
	else if( recipeName == "subroutine") {
		scene = std::unique_ptr<Scene>(new SceneSubroutine());
	}
	else if( recipeName == "two-side" ) {
		scene = std::unique_ptr<Scene>(new SceneTwoSide());
	}

	return runner.run(*scene);
}