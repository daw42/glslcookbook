#include "scenerunner.h"
#include "scenemandelbrot.h"
#include "scenecloth.h"
#include "sceneparticles.h"
#include "sceneedge.h"

#include <memory>

std::map<std::string, std::string> sceneInfo = {
        { "particles", "Simple particle simulation" },
        { "mandelbrot", "Mandelbrot set with compute shader" },
        { "cloth", "Cloth simulation with compute shader" },
        { "edge", "Edge detection filter using compute shader" }
};

int main(int argc, char *argv[])
{
	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneInfo);

	SceneRunner runner("Chapter 10 - " + recipe);

	std::unique_ptr<Scene> scene;
    if( recipe == "mandelbrot" ) {
        scene = std::unique_ptr<Scene>( new SceneMandelbrot() );
    } else if( recipe == "cloth" ) {
        scene = std::unique_ptr<Scene>( new SceneCloth() );
    } else if( recipe == "particles" ) {
        scene = std::unique_ptr<Scene>( new SceneParticles() );
    } else if( recipe == "edge" ) {
        scene = std::unique_ptr<Scene>( new SceneEdge() );
    } else {
        printf("Unknown recipe: %s\n", recipe.c_str());
        exit(EXIT_FAILURE);
    }

    return runner.run(*scene);
}
