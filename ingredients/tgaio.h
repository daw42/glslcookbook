#ifndef _TGAIO_H
#define _TGAIO_H

#include "cookbookogl.h"

#include <stdexcept>
#include <string>

class ifstream;
class ofstream;

namespace TGAIO {
    
    class IOException : public std::runtime_error {
    public:
        IOException( const std::string & msg ) :
                std::runtime_error(msg) { }
    };

    namespace LE {
	/**
	 * Read a little-endian short (2 bytes) from stream and return
	 * the value as an int.
	 * @param stream the ifstream to read from.
	 */
	int readShort(ifstream & stream);
	
	void writeShort(ofstream & stream, int value);
	
	/**
	 * Read a little-endian int (4 bytes) from stream and return
	 * the value.
	 * @param stream the ifstream to read from.
	 */
	int readInt( ifstream & stream );
	
    }
    
    GLubyte * read( const char * fName, /*out*/ int & width, /*out*/ int & height ) ;
    void write( GLubyte * pixelData, int width, int height, const char * fName );
    
    GLuint loadTex( const char * fileName, GLint &width /*out*/, GLint &height /*out*/ );
    
    /**
     * Loads a TGA file into an OpenGL texture.  This method only supports
     * 24 or 32 bpp images.
     * @param fileName the file name of the TGA file.
     * @return the texture ID
     */
    GLuint loadTex( const char * fileName );
}



#endif