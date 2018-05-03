#include "scenerunner.h"
#include "sceneao.h"
#include "scenejitter.h"
#include "scenepcf.h"
#include "sceneshadowmap.h"
#include "sceneshadowvolume.h"

std::map<std::string, std::string> sceneInfo = {
			{ "ao", "Ambient occlusion from a texture" },
			{ "jitter", "Blur shadow map edges using a random jitter" },
			{ "pcf", "Blur shadow map edges using percentage-closer-filtering" },
			{ "shadow-map", "Simple shadow map" },
			{ "shadow-volume", "Shadow Volumes using geometry shader." }
};

int main(int argc, char *argv[])
{
	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneInfo);

	SceneRunner runner("Chapter 7 - " + recipe);
	std::unique_ptr<Scene> scene;
	if( recipe == "ao" ) {
		scene = std::unique_ptr<Scene>( new SceneAo() );
	} else if( recipe == "jitter") {
		scene = std::unique_ptr<Scene>( new SceneJitter() );
	} else if( recipe == "pcf") {
		scene = std::unique_ptr<Scene>( new ScenePcf() );
	} else if( recipe == "shadow-map" ) {
		scene = std::unique_ptr<Scene>( new SceneShadowMap() );
	} else if( recipe == "shadow-volume" ) {
		scene = std::unique_ptr<Scene>( new SceneShadowVolume() );
	}

	return runner.run(*scene);
}