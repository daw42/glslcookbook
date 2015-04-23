#include "tgaio.h"

#include <fstream>
using std::ifstream;
using std::ofstream;

namespace TGAIO {

namespace LE {

int readShort(ifstream & stream) {
	unsigned char ic;     // input char
	int result;           // result

	ic = stream.get();
	result = ic;
	ic = stream.get();
	result |= ((unsigned int) ic << 8);
	return result;
}

void writeShort(ofstream & stream, int value) {
	unsigned char lowOrder = (unsigned char)value;
	unsigned char highOrder = (unsigned char)(value >> 8);

	stream.put(lowOrder);
	stream.put(highOrder);
}

int readInt(ifstream & fstr) {
	int result = 0;
	unsigned char ic = 0;

	ic = fstr.get();
	result = ic;
	ic = fstr.get();
	result |= ((unsigned int) ic << 8);
	ic = fstr.get();
	result |= ((unsigned int) ic << 16);
	ic = fstr.get();
	result |= ((unsigned int) ic << 24);
	return result;
}

} // Namespace LE

GLubyte * read( const char * fName, int & width, int & height ) {
	
	// Open file for reading
	std::ifstream inFile(fName, std::ios::binary);
	
	try {
		if (!inFile) {
			std::string msg = std::string("Error: can't open ") + fName ;
			throw IOException(msg);
		}
		
		int idLen = inFile.get();      // Length of image ID field
		int mapType = inFile.get();    // Color map type (expect 0 == no color map)
		int typeCode = inFile.get();   // Image type code (expect 2 == uncompressed)
		inFile.ignore(5);              // Color map info (ignored)
		int xOrigin = LE::readShort(inFile);  // X origin
		int yOrigin = LE::readShort(inFile);  // Y origin
		width = LE::readShort(inFile);        // Image width
		height = LE::readShort(inFile);       // Image height
		int bpp = inFile.get();               // Bits per pixel (expect 24 or 32)
		inFile.ignore();                      // Image descriptor (expect 0 for 24bpp and 8 for 32bpp)
		
		if( typeCode != 2 || mapType != 0 ) {
			throw IOException("File does not appear to be a non-color-mapped, uncompressed TGA image");
		}
		
		if( bpp != 24 && bpp != 32 ) {
			throw IOException("File must be 24 or 32 bits per pixel.\n");
		}
		
		// Skip the image ID data
		if( idLen > 0 ) inFile.ignore(idLen);
		
		// Color map data would be here, but we assume no color map
		
		printf("%s: (%d x %d) %d bpp origin (%d, %d)\n", fName, width, height, bpp,
		       xOrigin, yOrigin);
		
		// Read pixel data
		GLubyte *p = new GLubyte[width * height * 4];
		// 24 bpp -- Blue, Green, Red
		// 32 bpp -- Blue, Green, Red, Alpha
		// p -- stored as RGBA
		for( unsigned int i = 0; i < (unsigned int)(width * height) ; i++ )
		{
			p[i*4 + 2] = inFile.get();  // Blue
			p[i*4 + 1] = inFile.get();  // Green
			p[i*4    ] = inFile.get();  // Red 
			if( bpp == 32 )
				p[i*4 + 3] = inFile.get();
			else
				p[i*4 + 3] = 0xFF;
		}
		
		inFile.close();
		return p;
	}
	catch (IOException & e) {
		inFile.close();
		throw e;
	}
}

void write( GLubyte * pd, int width, int height, const char * fName) {
	
	std::ofstream oFile(fName, std::ios::binary);
	
	try {
		if( ! oFile ) {
			std::string msg = std::string("Unable to open file ") + fName +
				std::string(" for writing.");
			throw IOException(msg);
		}
		
		const char zero[] = {0,0,0,0,0};
		
		oFile.put(0);          // Length of image ID field
		oFile.put(0);          // Color map type (0 == no color map)
		oFile.put(2);          // Image type code (2 == uncompressed)
		oFile.write(zero, 5);  // Color map info (ignored)
		oFile.write(zero, 2);  // X origin (0)
		oFile.write(zero, 2);  // Y origin (0)
		LE::writeShort(oFile, width);    // Image width
		LE::writeShort(oFile, height);   // Image height
		oFile.put(32);               // Bits per pixel (32)
		oFile.put(8);                // Image descriptor (8 => 32bpp)
		
		for( int i = 0; i < (width * height); i++ ) {
			oFile.put( pd[i*4 + 2] );  // Blue
			oFile.put( pd[i*4 + 1] );  // Green
			oFile.put( pd[i*4    ] );  // Red 
			oFile.put( pd[i*4 + 3] );  // alpha
		}
		
		oFile.close();
		
	} catch(IOException & e) {
		oFile.close();
		throw e;
	}
	
	
}

GLuint loadTex(const char* fName, GLint & width, GLint &height) {
	GLubyte * data = TGAIO::read(fName, width, height);

	GLuint texID;
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);

        // Allocate storage
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        // Copy data into storage
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	delete [] data;

	return texID;
}

GLuint loadTex(const char* fName) {
	GLint w,h;
	return TGAIO::loadTex(fName, w, h);
}


} // Namespace TGAIO
