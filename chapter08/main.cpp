#include "scenerunner.h"
#include "scenedecay.h"
#include "scenenightvision.h"
#include "scenepaint.h"
#include "scenesky.h"
#include "scenewood.h"
#include "scenenoise.h"

#include <memory>

std::map<std::string, std::string> sceneInfo = {
		{ "noise", "just display the raw noise texture" },
		{ "decay", "decay of a teapot" },
		{ "night-vision", "night vision goggles" },
		{ "paint", "paint spatters on a teapot" },
		{ "sky", "clouds and sky" },
		{ "wood", "wood " }
};

int main(int argc, char *argv[])
{
	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneInfo);

	SceneRunner runner("Chapter 8 - " + recipe);

	std::unique_ptr<Scene> scene;
	if( recipe == "decay" ) {
		scene = std::unique_ptr<Scene>( new SceneDecay() );
	} else if( recipe == "noise" ) {
		scene = std::unique_ptr<Scene>( new SceneNoise() );
	} else if( recipe == "night-vision") {
		scene = std::unique_ptr<Scene>( new SceneNightVision() );
	} else if( recipe == "paint") {
		scene = std::unique_ptr<Scene>( new ScenePaint() );
	} else if( recipe == "sky" ) {
		scene = std::unique_ptr<Scene>( new SceneSky() );
	} else if( recipe == "wood" ) {
		scene = std::unique_ptr<Scene>( new SceneWood() );
	} else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		exit(EXIT_FAILURE);
	}

	return runner.run(*scene);
}
