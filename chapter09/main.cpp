#include "scenerunner.h"
#include "scenefire.h"
#include "sceneparticles.h"
#include "sceneparticlesfeedback.h"
#include "sceneparticlesinstanced.h"
#include "scenesmoke.h"
#include "scenewave.h"

#include <memory>

std::map<std::string, std::string> sceneInfo = {
		{ "fire", "particles simulating fire" },
		{ "particles", "a fountain of particles" },
		{ "particles-feedback", "a fountain of particles implemented with transform feedback" },
		{ "particles-instanced", "a fountain of instanced particles, mmmm.. donuts" },
		{ "smoke", "particles simulating smoke" },
		{ "wave", "a plane wave displacement animation" }
};

int main(int argc, char *argv[])
{
	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneInfo);

	SceneRunner runner("Chapter 9 - " + recipe);

	std::unique_ptr<Scene> scene;
	if( recipe == "fire" ) {
		scene = std::unique_ptr<Scene>( new SceneFire() );
	} else if( recipe == "particles") {
		scene = std::unique_ptr<Scene>( new SceneParticles() );
	} else if( recipe == "particles-feedback") {
		scene = std::unique_ptr<Scene>( new SceneParticlesFeedback() );
	} else if( recipe == "particles-instanced" ) {
		scene = std::unique_ptr<Scene>( new SceneParticlesInstanced() );
	} else if( recipe == "smoke" ) {
		scene = std::unique_ptr<Scene>( new SceneSmoke() );
	} else if( recipe == "wave" ) {
		scene = std::unique_ptr<Scene>( new SceneWave() );
	} else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		exit(EXIT_FAILURE);
	}

	return runner.run(*scene);
}
