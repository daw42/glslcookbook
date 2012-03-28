#include "sceneao.h"

#include <cstdio>

#include <glimg/glimg.h>

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneAo::SceneAo()
{
    width = 800;
    height = 600;
}

void SceneAo::initScene()
{
    compileAndLinkShader();

    glClearColor(0.5f,0.5f,0.5f,1.0f);

    glEnable(GL_DEPTH_TEST);

    angle = (float)(PI/2.0f);

    //teapot = new VBOTeapot(14, mat4(1.0f));
    //plane = new VBOPlane(20.0f, 20.0f, 2, 2);
    //float scale = 2.0f;
    //torus = new VBOTorus(0.7f * scale,0.3f * scale,50,50);
    ogre = new VBOMesh("../media/bs_ears.obj", false, true);

//    lightPos = vec3(0.0f,10.5f,15.0f);  // World coords
//    lightView = glm::lookAt(lightPos, vec3(0.0f), vec3(0.0f,1.0f,0.0f));
//    lightProjection = glm::perspective(50.0f, 1.0f, 0.1f, 100.0f);
//    lightPV = shadowScale * lightProjection * lightView;

    lightPos = glm::vec4(0.0f,0.0f,0.0f,1.0f);  // Camera coords

    prog.setUniform("Light.Position", lightPos);
    prog.setUniform("Light.Intensity", vec3(1.0f));

    float c = 2.25f;
    projection = glm::ortho(-0.4f * c, 0.4f * c, -0.3f *c, 0.3f*c, 0.1f, 100.0f);

    const char * texName = "../media/texture/ao_ears.png";
	try {
		glimg::ImageSet * imgSet;
		imgSet = glimg::loaders::stb::LoadFromFile(texName);
		glimg::SingleImage &img = imgSet->GetImage(0);
		glimg::OpenGLPixelTransferParams params = glimg::GetUploadFormatType(img.GetFormat(), 0);
		glimg::Dimensions dims = img.GetDimensions();

		glPixelStorei(GL_UNPACK_ALIGNMENT, img.GetFormat().LineAlign());

		// Copy file to OpenGL
		glActiveTexture(GL_TEXTURE0);
		GLuint tid;
		glGenTextures(1, &tid);
		glBindTexture(GL_TEXTURE_2D, tid);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dims.width, dims.height, 0,
					 params.format, params.type, img.GetImageData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		delete imgSet;

	} catch( glimg::loaders::stb::StbLoaderException &e ) {
		fprintf(stderr, "Unable to load texture %s: %s\n", texName, e.what());
		exit(1);
	}
    prog.setUniform("AOTex", 0);

    const char * diffTexName = "../media/texture/diffuse.png";
	try {
		glimg::ImageSet * imgSet;
		imgSet = glimg::loaders::stb::LoadFromFile(diffTexName);
		glimg::SingleImage &img = imgSet->GetImage(0);
		glimg::OpenGLPixelTransferParams params = glimg::GetUploadFormatType(img.GetFormat(), 0);
		glimg::Dimensions dims = img.GetDimensions();

		glPixelStorei(GL_UNPACK_ALIGNMENT, img.GetFormat().LineAlign());

		// Copy file to OpenGL
		glActiveTexture(GL_TEXTURE1);
		GLuint tid;
		glGenTextures(1, &tid);
		glBindTexture(GL_TEXTURE_2D, tid);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dims.width, dims.height, 0,
					 params.format, params.type, img.GetImageData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		delete imgSet;

	} catch( glimg::loaders::stb::StbLoaderException &e ) {
		fprintf(stderr, "Unable to load texture %s: %s\n", texName, e.what());
		exit(1);
	}
    prog.setUniform("DiffTex", 1);
}

void SceneAo::update( float t )
{
    angle += 0.003f;
    if( angle > TWOPI_F) angle -= TWOPI_F;
}

void SceneAo::render()
{
    view = glm::lookAt(vec3(3.0f * cos(angle),0.0f,3.0f * sin(angle)), vec3(0.0f), vec3(0.0f,1.0f,0.0f));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    glFinish();
}

void SceneAo::drawScene()
{
    model = mat4(1.0f);
    setMatrices();
    ogre->render();

    glFinish();
}

void SceneAo::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneAo::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
}

void SceneAo::compileAndLinkShader()
{
    if( ! prog.compileShaderFromFile("shader/ao.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/ao.fs",GLSLShader::FRAGMENT))
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
