#include "sceneprojtex.h"

#include <cstdio>
#include <cstdlib>

#include <iostream>
using std::cout;
using std::endl;

#include <glimg/glimg.h>

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneProjTex::SceneProjTex()
{
}

void SceneProjTex::initScene()
{
    compileAndLinkShader();

    glEnable(GL_DEPTH_TEST);

    teapot = new VBOTeapot(14, mat4(1.0f));
    plane = new VBOPlane(100.0f,100.0f,1,1);

    projection = mat4(1.0f);

    angle = (float)( TO_RADIANS(90.0) );

    vec3 projPos = vec3(2.0f,5.0f,5.0f);
    vec3 projAt = vec3(-2.0f,-4.0f,0.0f);
    vec3 projUp = vec3(0.0f,1.0f,0.0f);
    mat4 projView = glm::lookAt(projPos, projAt, projUp);
    mat4 projProj = glm::perspective(30.0f, 1.0f, 0.2f, 1000.0f);
    mat4 projScaleTrans = glm::translate(vec3(0.5f)) * glm::scale(vec3(0.5f));
    prog.setUniform("ProjectorMatrix", projScaleTrans * projProj * projView);

    // Load texture file
    const char * texName = "../media/texture/flower.png";
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		delete imgSet;

	} catch( glimg::loaders::stb::StbLoaderException &e ) {
		fprintf(stderr, "Unable to load texture %s: %s\n", texName, e.what());
		exit(1);
	}
    
    prog.setUniform("ProjectorTex", 0);
    prog.setUniform("Light.Position", vec4(0.0f,0.0f,0.0f,1.0f) );
    prog.setUniform("Light.Intensity", vec3(1.0f,1.0f,1.0f));
}

void SceneProjTex::update( float t )
{
    angle += 0.0001f;
    if( angle > TWOPI) angle -= TWOPI;
}

void SceneProjTex::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vec3 cameraPos = vec3( 7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));

    prog.setUniform("Material.Kd", 0.5f, 0.2f, 0.1f);
    prog.setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 100.0f);

    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,-1.0f,0.0f));
    model *= glm::rotate(-90.0f, vec3(1.0f,0.0f,0.0f));
    setMatrices();
    teapot->render();

    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 1.0f);
    model = mat4(1.0f);
    model *= glm::translate(vec3(0.0f,-0.75f,0.0f));
    setMatrices();
    plane->render();
}

void SceneProjTex::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelMatrix", model);
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix",
                    mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneProjTex::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(50.0f, (float)w/h, 0.3f, 1000.0f);
    //float c = 2.0f;
    //projection = glm::ortho( -0.4f * c, 0.4f * c, -0.3f * c, 0.3f * c, 0.1f, 100.0f);
}

void SceneProjTex::compileAndLinkShader()
{
    if( ! prog.compileShaderFromFile("shader/projtex.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/projtex.fs",GLSLShader::FRAGMENT))
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
