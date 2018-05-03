#include "scenerunner.h"
#include "scenebezcurve.h"
#include "scenepointsprite.h"
#include "scenequadtess.h"
#include "sceneshadewire.h"
#include "scenesilhouette.h"
#include "scenetessteapot.h"
#include "scenetessteapotdepth.h"

#include <map>

std::map<std::string, std::string> sceneInfo = {
            { "bez-curve", "2D Bezier curve with tessellation shader" },
            { "point-sprite", "Point sprites with the geometry shader" },
            { "quad-tess", "Demonstrates how quad tessellation works" },
            { "shade-wire", "Uses the geometry shader to draw a mesh over a shaded object" },
            { "silhouette", "Uses the geometry shader to draw silhouette edges" },
            { "tess-teapot", "Uses tessellation to draw a teapot" },
            { "tess-teapot-depth", "Varies the amount of tessellation with depth" }
};

int main(int argc, char *argv[])
{
	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneInfo);

	SceneRunner runner("Chapter 6 - " + recipe);

	std::unique_ptr<Scene> scene;
    if( recipe == "bez-curve" ) {
        scene = std::unique_ptr<Scene>( new SceneBezCurve() );
    } else if( recipe == "point-sprite") {
        scene = std::unique_ptr<Scene>( new ScenePointSprite() );
    } else if( recipe == "quad-tess") {
        scene = std::unique_ptr<Scene>( new SceneQuadTess() );
    } else if( recipe == "shade-wire" ) {
        scene = std::unique_ptr<Scene>( new SceneShadeWire() );
    } else if( recipe == "silhouette") {
        scene = std::unique_ptr<Scene>( new SceneSilhouette() );
    } else if( recipe == "tess-teapot" ) {
        scene = std::unique_ptr<Scene>( new SceneTessTeapot() );
    } else if( recipe == "tess-teapot-depth" ) {
        scene = std::unique_ptr<Scene>( new SceneTessTeapotDepth() );
    } else {
        printf("Unknown recipe: %s\n", recipe.c_str());
        exit(EXIT_FAILURE);
    }

    return runner.run(*scene);
}
