#include "scene.h"
#include "scenerunner.h"
#include "scenedirectional.h"
#include "scenefog.h"
#include "scenemultilight.h"
#include "sceneperfragment.h"
#include "scenespot.h"
#include "scenetoon.h"


std::map<std::string, std::string> sceneInfo = {
	{ "directional", "Directional light source" },
	{ "fog", "Fog" },
	{ "multi-light", "Multiple light sources" },
	{ "per-frag", "Per-fragment shading" },
	{ "spot", "Spot light" },
	{ "toon", "Toon shading" }
};


int main(int argc, char *argv[])
{
	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneInfo);

	SceneRunner runner("Chapter 3 - " + recipe);
	std::unique_ptr<Scene> scene;
	if( recipe == "directional" ) {
		scene = std::unique_ptr<Scene>( new SceneDirectional() );
	} else if( recipe == "fog") {
		scene = std::unique_ptr<Scene>( new SceneFog() );
	} else if( recipe == "multi-light") {
		scene = std::unique_ptr<Scene>( new SceneMultiLight() );
	} else if( recipe == "per-frag" ) {
		scene = std::unique_ptr<Scene>( new ScenePerFragment() );
	} else if( recipe == "spot") {
		scene = std::unique_ptr<Scene>( new SceneSpot() );
	} else if( recipe == "toon" ) {
		scene = std::unique_ptr<Scene>( new SceneToon() );
	}

	return runner.run(*scene);
}
