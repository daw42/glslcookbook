#include "glslprogram.h"

#include "glutils.h"

#include <fstream>
using std::ifstream;
using std::ios;

#include <sstream>
#include <sys/stat.h>

GLSLProgram::GLSLProgram() : handle(0), linked(false) { }

GLSLProgram::~GLSLProgram() {
	if(handle == 0) return;

	// Query the number of attached shaders
	GLint numShaders = 0;
	glGetProgramiv(handle, GL_ATTACHED_SHADERS, &numShaders);

	// Get the shader names
	GLuint * shaderNames = new GLuint[numShaders];
	glGetAttachedShaders(handle, numShaders, NULL, shaderNames);

	// Delete the shaders
	for (int i = 0; i < numShaders; i++)
		glDeleteShader(shaderNames[i]);

	// Delete the program
	glDeleteProgram (handle);
	
	delete[] shaderNames;
}

void GLSLProgram::compileShader( const char * fileName,
                                 GLSLShader::GLSLShaderType type )
                                 throw( GLSLProgramException )
{
    if( ! fileExists(fileName) )
    {
        string message = string("Shader: ") + fileName + " not found.";
        throw GLSLProgramException(message);
    }

    if( handle <= 0 ) {
        handle = glCreateProgram();
        if( handle == 0) {
            throw GLSLProgramException("Unable to create shader program.");
        }
    }

    ifstream inFile( fileName, ios::in );
    if( !inFile ) {
    	string message = string("Unable to open: ") + fileName;
		throw GLSLProgramException(message);
    }

	// Get file contents
	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();

    compileShader(code.str(), type, fileName);
}

void GLSLProgram::compileShader( const string & source, 
                                 GLSLShader::GLSLShaderType type,
                                 const char * fileName )
                                 throw(GLSLProgramException)
{
    if( handle <= 0 ) {
        handle = glCreateProgram();
        if( handle == 0) {
            throw GLSLProgramException("Unable to create shader program.");
        }
    }

    GLuint shaderHandle = 0;

    switch( type ) {
    case GLSLShader::VERTEX:
        shaderHandle = glCreateShader(GL_VERTEX_SHADER);
        break;
    case GLSLShader::FRAGMENT:
        shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    case GLSLShader::GEOMETRY:
        shaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
        break;
    case GLSLShader::TESS_CONTROL:
        shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
        break;
    case GLSLShader::TESS_EVALUATION:
        shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
        break;
    default:
        throw GLSLProgramException("Invalid shader type");
    }

    const char * c_code = source.c_str();
    glShaderSource( shaderHandle, 1, &c_code, NULL );

    // Compile the shader
    glCompileShader(shaderHandle);

    // Check for errors
    int result;
    glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result ) {
        // Compile failed, get log
        int length = 0;
        string logString;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length );
        if( length > 0 ) {
            char * c_log = new char[length];
            int written = 0;
            glGetShaderInfoLog(shaderHandle, length, &written, c_log);
            logString = c_log;
            delete [] c_log;
        }
        string msg;
        if( fileName ) {
        	msg = string(fileName) + ": shader compliation failed\n";
        } else {
        	msg = "Shader compilation failed.\n";
        }
        msg += logString;

        throw GLSLProgramException(msg);
        
    } else {
        // Compile succeeded, attach shader
        glAttachShader(handle, shaderHandle);
    }
}

void GLSLProgram::link() throw(GLSLProgramException)
{
    if( linked ) return;
    if( handle <= 0 ) 
    	throw GLSLProgramException("Program has not been compiled.");

    glLinkProgram(handle);

    int status = 0;
    glGetProgramiv( handle, GL_LINK_STATUS, &status);
    if( GL_FALSE == status ) {
        // Store log and return false
        int length = 0;
        string logString;

        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 ) {
            char * c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog(handle, length, &written, c_log);
            logString = c_log;
            delete [] c_log;
        }

        throw GLSLProgramException(string("Program link failed:\n") + logString);
    } else {
        linked = true;
    }
    
}

void GLSLProgram::use() throw(GLSLProgramException)
{
    if( handle <= 0 || (! linked) ) 
    	throw GLSLProgramException("Shader has not been linked");
    glUseProgram( handle );
}

int GLSLProgram::getHandle()
{
    return handle;
}

bool GLSLProgram::isLinked()
{
    return linked;
}

void GLSLProgram::bindAttribLocation( GLuint location, const char * name)
{
    glBindAttribLocation(handle, location, name);
}

void GLSLProgram::bindFragDataLocation( GLuint location, const char * name )
{
    glBindFragDataLocation(handle, location, name);
}

void GLSLProgram::setUniform( const char *name, float x, float y, float z)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ) {
        glUniform3f(loc,x,y,z);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, const vec3 & v)
{
    this->setUniform(name,v.x,v.y,v.z);
}

void GLSLProgram::setUniform( const char *name, const vec4 & v)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ) {
        glUniform4f(loc,v.x,v.y,v.z,v.w);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, const vec2 & v)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 ) {
        glUniform2f(loc,v.x,v.y);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, const mat4 & m)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, const mat3 & m)
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, float val )
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniform1f(loc, val);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, int val )
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniform1i(loc, val);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::setUniform( const char *name, bool val )
{
    int loc = getUniformLocation(name);
    if( loc >= 0 )
    {
        glUniform1i(loc, val);
    } else {
        printf("Uniform: %s not found.\n",name);
    }
}

void GLSLProgram::printActiveUniforms() {

    GLint nUniforms, size, location, maxLen;
    GLchar * name;
    GLsizei written;
    GLenum type;

    glGetProgramiv( handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
    glGetProgramiv( handle, GL_ACTIVE_UNIFORMS, &nUniforms);

    name = (GLchar *) malloc( maxLen );

    printf(" Location | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nUniforms; ++i ) {
        glGetActiveUniform( handle, i, maxLen, &written, &size, &type, name );
        location = glGetUniformLocation(handle, name);
        printf(" %-8d | %s\n",location, name);
    }

    free(name);
}

void GLSLProgram::printActiveAttribs() {

    GLint written, size, location, maxLength, nAttribs;
    GLenum type;
    GLchar * name;

    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &nAttribs);

    name = (GLchar *) malloc( maxLength );

    printf(" Index | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nAttribs; i++ ) {
        glGetActiveAttrib( handle, i, maxLength, &written, &size, &type, name );
        location = glGetAttribLocation(handle, name);
        printf(" %-5d | %s\n",location, name);
    }

    free(name);
}

void GLSLProgram::validate() throw(GLSLProgramException)
{
    if( ! isLinked() ) 
    	throw GLSLProgramException("Program is not linked");

    GLint status;
    glValidateProgram( handle );
    glGetProgramiv( handle, GL_VALIDATE_STATUS, &status );

    if( GL_FALSE == status ) {
        // Store log and return false
        int length = 0;
        string logString;

        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 ) {
            char * c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog(handle, length, &written, c_log);
            logString = c_log;
            delete [] c_log;
        }

        throw GLSLProgramException(string("Program failed to validate\n") + logString);
        
    }
}

int GLSLProgram::getUniformLocation(const char * name )
{
    return glGetUniformLocation(handle, name);
}

bool GLSLProgram::fileExists( const string & fileName )
{
    struct stat info;
    int ret = -1;

    ret = stat(fileName.c_str(), &info);
    return 0 == ret;
}
