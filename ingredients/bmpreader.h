#ifndef BMPREADER_H_
#define BMPREADER_H_

#include <fstream>
using std::ifstream;

#include "cookbookogl.h"

class BMPReader {
public:

	/**
	 * Loads a BMP file into an array suitable for loading into an
	 * OpenGL texture.  This method only supports 24 or 32 bpp images.
	 * The array that is returned should be deleted when one is finished
	 * with it.  Data is stored in the array as RGBA, 4 bytes per
	 * pixel.
	 * @param fileName the file name of the BMP file.
	 * @param width the width in pixels of the image is stored here.
	 * @param height the height in pixels of the image is stored here.
	 * @return a pointer to the image data
	 */
	static GLubyte * load( const char * fileName, GLuint &width /*out*/, GLuint &height /*out*/ );

	/**
	 * Loads a BMP file into an OpenGL texture.  This method only supports
	 * 24 or 32 bpp images.
	 * @param fileName the file name of the BMP file.
	 * @param width the width in pixels of the image is stored here.
	 * @param height the height in pixels of the image is stored here.
	 * @return the texture ID
	 */
	static GLuint loadTex( const char * fileName, GLuint &width /*out*/, GLuint &height /*out*/ );
	static GLuint loadTex( const char * fileName );
private:
	/**
	 * Read a little-endian short (2 bytes) from stream and return
	 * the value as an int.
	 * @param stream the ifstream to read from.
	 */
	static int readShortLE(ifstream & stream);

	/**
	 * Read a little-endian int (4 bytes) from stream and return
	 * the value.
	 * @param stream the ifstream to read from.
	 */
	static int readIntLE( ifstream & stream );
};

#endif
