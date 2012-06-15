#include "scenefire.h"

#include <cstdio>
#include <cstdlib>

#include <glimg/glimg.h>

#include "glutils.h"
#include "defines.h"

using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

SceneFire::SceneFire()
{
    width = 800;
    height = 600;
    drawBuf = 1;
    time = 0.0f;
    deltaT = 0.0f;
}

void SceneFire::initScene()
{    

    compileAndLinkShader();

    GLuint programHandle = prog.getHandle();
    renderSub = glGetSubroutineIndex(programHandle, GL_VERTEX_SHADER, "render");
    updateSub = glGetSubroutineIndex(programHandle, GL_VERTEX_SHADER, "update");

    glClearColor(0.1f,0.1f,0.1f,0.0f);

    //glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(50.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    plane = new VBOPlane(13.0f, 10.0f, 200, 2);
    projection = mat4(1.0f);

    angle = (float)(PI / 2.0f);
    model = mat4(1.0f);

    initBuffers();

    glGenQueries(1, &query);

    const char * texName = "../media/texture/fire.png";
	try {
		glimg::ImageSet * imgSet;
		imgSet = glimg::loaders::stb::LoadFromFile(texName);
		const glimg::SingleImage &img = imgSet->GetImage(0);
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

	prog.setUniform("ParticleTex", 0);
    prog.setUniform("ParticleLifetime", 4.0f);
    prog.setUniform("Accel", vec3(0.0f,0.1f,0.0f));
    setMatrices();
}

void SceneFire::initBuffers()
{
    nParticles = 4000;

    // Generate the buffers
    glGenBuffers(2, posBuf);    // position buffers
    glGenBuffers(2, velBuf);    // velocity buffers
    glGenBuffers(2, startTime); // Start time buffers
    glGenBuffers(1, &initVel);  // Initial velocity buffer (never changes, only need one)

    // Allocate space for all buffers
    int size = nParticles * 3 * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
    glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_DYNAMIC_COPY);

    // Fill the first position buffer with zeroes
    GLfloat *data = new GLfloat[nParticles * 3];
    for( int i = 0; i < nParticles * 3; i += 3 ) {
        data[i] = glm::mix(-2.0f, 2.0f, randFloat());
        data[i+1] = 0.0f;
        data[i+2] = 0.0f;
    }
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

    // Fill the first velocity buffer with random velocities
    for( int i = 0; i < nParticles; i++ ) {
        data[3*i]   = 0.0f;
        data[3*i+1] = glm::mix(0.1f,0.5f,randFloat());
        data[3*i+2] = 0.0f;
    }
    glBindBuffer(GL_ARRAY_BUFFER,velBuf[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    glBindBuffer(GL_ARRAY_BUFFER,initVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

    // Fill the first start time buffer
    delete [] data;
    data = new GLfloat[nParticles];
    float time = 0.0f;
    float rate = 0.001f;
    for( int i = 0; i < nParticles; i++ ) {
        data[i] = time;
        time += rate;
    }
    glBindBuffer(GL_ARRAY_BUFFER,startTime[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    delete [] data;

    // Create vertex arrays for each set of buffers
    glGenVertexArrays(2, particleArray);

    // Set up particle array 0
    glBindVertexArray(particleArray[0]);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, startTime[0]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    // Set up particle array 1
    glBindVertexArray(particleArray[1]);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, startTime[1]);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    // Setup the feedback objects
    glGenTransformFeedbacks(2, feedback);

    // Transform feedback 0
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[0]);

    // Transform feedback 1
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, startTime[1]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

float SceneFire::randFloat() {
    return ((float)rand() / RAND_MAX);
}

void SceneFire::update( float t )
{
    deltaT = t - time;
    time = t;
}

void SceneFire::render()
{
    // Update pass
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &updateSub);

    prog.setUniform("Time", time);
    prog.setUniform("H", deltaT);

    glEnable(GL_RASTERIZER_DISCARD);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);

    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);

    glBeginTransformFeedback(GL_POINTS);
      glBindVertexArray(particleArray[1-drawBuf]);
      glDrawArrays(GL_POINTS, 0, nParticles);
    glEndTransformFeedback();

    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

    GLuint ptsWritten;
    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &ptsWritten);
    //printf("Written: %d\n", ptsWritten);
    glDisable(GL_RASTERIZER_DISCARD);

    // Render pass
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderSub);
    glClear( GL_COLOR_BUFFER_BIT );
    view = glm::lookAt(vec3(3.0f * cos(angle),1.5f,3.0f * sin(angle)), vec3(0.0f,1.5f,0.0f), vec3(0.0f,1.0f,0.0f));
    setMatrices();

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    glBindVertexArray(particleArray[drawBuf]);
    glDrawArrays(GL_POINTS, 0, nParticles);

    // Swap buffers
    drawBuf = 1 - drawBuf;
}

void SceneFire::setMatrices()
{
    mat4 mv = view * model;
    //prog.setUniform("ModelViewMatrix", mv);
   // prog.setUniform("NormalMatrix",
   //                 mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
    prog.setUniform("MVP", projection * mv);
}

void SceneFire::resize(int w, int h)
{
    glViewport(0,0,w,h);
    width = w;
    height = h;
    projection = glm::perspective(60.0f, (float)w/h, 0.3f, 100.0f);
}

void SceneFire::compileAndLinkShader()
{
    if( ! prog.compileShaderFromFile("shader/fire.vs",GLSLShader::VERTEX) )
    {
        printf("Vertex shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if( ! prog.compileShaderFromFile("shader/fire.fs",GLSLShader::FRAGMENT))
    {
        printf("Fragment shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }

    //////////////////////////////////////////////////////
    // Setup the transform feedback (must be done before linking the program)
    GLuint progHandle = prog.getHandle();
    const char * outputNames[] = { "Position", "Velocity", "StartTime" };
    glTransformFeedbackVaryings(progHandle, 3, outputNames, GL_SEPARATE_ATTRIBS);
    ///////////////////////////////////////////////////////

    if( ! prog.link() )
    {
        printf("Shader program failed to link!\n%s",
               prog.log().c_str());
        exit(1);
    }

    prog.use();
}
