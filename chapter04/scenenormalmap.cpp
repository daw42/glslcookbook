#include "scenenormalmap.h"

#include <cstdio>
#include <cstdlib>

#include <glimg/glimg.h>

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneNormalMap::SceneNormalMap()
{
}

void SceneNormalMap::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    ogre = new VBOMesh("../media/bs_ears.obj",false,true,true);

    view = glm::lookAt(vec3(-1.0f,0.25f,2.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
    projection = mat4(1.0f);

    angle = (float)( TO_RADIANS(100.0) );

    prog.setUniform("Light.Intensity", vec3(0.9f,0.9f,0.9f) );

    GLuint texIDs[2];
    glGenTextures(2, texIDs);

    // Load brick texture file
    const char * texName = "../media/texture/ogre_diffuse.png";
	try {
		glimg::ImageSet * imgSet;
		imgSet = glimg::loaders::stb::LoadFromFile(texName);
		const glimg::SingleImage &img = imgSet->GetImage(0);
		glimg::OpenGLPixelTransferParams params = glimg::GetUploadFormatType(img.GetFormat(), 0);
		glimg::Dimensions dims = img.GetDimensions();

		glPixelStorei(GL_UNPACK_ALIGNMENT, img.GetFormat().LineAlign());

		// Copy file to OpenGL
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texIDs[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dims.width, dims.height, 0,
					 params.format, params.type, img.GetImageData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		delete imgSet;
	} catch( glimg::loaders::stb::StbLoaderException &e ) {
		fprintf(stderr, "Unable to load texture %s: %s\n", texName, e.what());
		exit(1);
	}

    // Load moss texture file
    texName = "../media/texture/ogre_normalmap.png";
	try {
		glimg::ImageSet * imgSet;
		imgSet = glimg::loaders::stb::LoadFromFile(texName);
		const glimg::SingleImage &img = imgSet->GetImage(0);
		glimg::OpenGLPixelTransferParams params = glimg::GetUploadFormatType(img.GetFormat(), 0);
		glimg::Dimensions dims = img.GetDimensions();

		glPixelStorei(GL_UNPACK_ALIGNMENT, img.GetFormat().LineAlign());

		// Copy file to OpenGL
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texIDs[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dims.width, dims.height, 0,
					 params.format, params.type, img.GetImageData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		delete imgSet;
	} catch( glimg::loaders::stb::StbLoaderException &e ) {
		fprintf(stderr, "Unable to load texture %s: %s\n", texName, e.what());
		exit(1);
	}
    
    prog.setUniform("ColorTex", 0);
    prog.setUniform("NormalMapTex", 1);
}

void SceneNormalMap::update( float t )
{
    angle += 0.0001f;
    if( angle > TWOPI) angle -= TWOPI;
}

void SceneNormalMap::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog.setUniform("Light.Position", view * vec4(10.0f * cos(angle),1.0f,10.0f * sin(angle),1.0f) );
    prog.setUniform("Material.Ks", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 1.0f);

    model = mat4(1.0f);
    setMatrices();
    ogre->render();
}

void SceneNormalMap::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneNormalMap::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    float c = 2.0f;
    projection = glm::ortho( -0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
}

void SceneNormalMap::compileAndLinkShader()
{
    if( ! prog.compileShaderFromFile("shader/normalmap.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/normalmap.fs",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.link() )
    {
        printf("Shader program failed to link!\n%s",
               prog.log().c_str());
        exit(1);
    }

    prog.use();
}
