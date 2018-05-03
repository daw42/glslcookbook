#include "scene.h"
#include "scenerunner.h"
#include "scenetexture.h"
#include "scenealphatest.h"
#include "scenemultitex.h"
#include "scenenormalmap.h"
#include "sceneprojtex.h"
#include "scenereflectcube.h"
#include "scenerefractcube.h"
#include "scenerendertotex.h"
#include "scenesamplerobj.h"

std::map<std::string, std::string> sceneInfo = {
	{ "alpha-test", "Discard fragments based on an alpha test." },
	{ "multi-tex", "Multiple textures" },
	{ "normal-map", "Normal map" },
	{ "proj-tex", "Projected texture" },
	{ "reflect-cube", "Reflection with a cube map" },
	{ "refract-cube", "Refraction with a cube map" },
	{ "render-to-tex", "Render to a texture using framebuffer objects" },
	{ "sampler-obj", "Sampler objects " },
	{ "texture", "Basic texture mapping" }
};

int main(int argc, char *argv[])
{

	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneInfo);

	SceneRunner runner("Chapter 4 - " + recipe);
	std::unique_ptr<Scene> scene;
	if( recipe == "alpha-test" ) {
		scene = std::unique_ptr<Scene>( new SceneAlphaTest() );
	} else if( recipe == "multi-tex") {
		scene = std::unique_ptr<Scene>( new SceneMultiTex() );
	} else if( recipe == "normal-map") {
		scene = std::unique_ptr<Scene>( new SceneNormalMap() );
	} else if( recipe == "proj-tex" ) {
		scene = std::unique_ptr<Scene>( new SceneProjTex() );
	} else if( recipe == "reflect-cube") {
		scene = std::unique_ptr<Scene>( new SceneReflectCube() );
	} else if( recipe == "refract-cube" ) {
		scene = std::unique_ptr<Scene>( new SceneRefractCube() );
	} else if( recipe == "render-to-tex" ) {
		scene = std::unique_ptr<Scene>( new SceneRenderToTex() );
	} else if( recipe == "sampler-obj" ) {
		scene = std::unique_ptr<Scene>( new SceneSamplerObj() );
	} else if( recipe == "texture" ) {
		scene = std::unique_ptr<Scene>( new SceneTexture() );
	} else {
		printf("Unknown recipe: %s\n", recipe.c_str());
		exit(EXIT_FAILURE);
	}

    return runner.run(*scene);
}
