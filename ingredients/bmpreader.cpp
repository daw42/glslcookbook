#include "bmpreader.h"

#include <string>
using std::string;

int BMPReader::readShortLE(ifstream & stream) {
	unsigned char ic;     // input char
	int result;           // result

	ic = stream.get();
	result = ic;
	ic = stream.get();
	result |= ((unsigned int) ic << 8);
	return result;
}

int BMPReader::readIntLE(ifstream & fstr) {
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

GLubyte* BMPReader::load(const char *filename, GLuint & width, GLuint & height) {
	char header[3];
	int fileSize, offset, numCols, numRows;
	int bpp;

	std::ifstream inFile(filename, std::ios::binary);
	if (!inFile) {
		printf("Error: can't open file: %s\n", filename);
		return NULL;
	}

	inFile.get(header, 3);
	string magic(header);
	if( magic != "BM" ) {
		printf("Unrecognized file format: %s\n", filename);
		inFile.close();
		return NULL;
	}

	fileSize = BMPReader::readIntLE(inFile);   // Total file size
	BMPReader::readShortLE(inFile);            // ignore, reserved
	BMPReader::readShortLE(inFile);            // ignore, reserved
	offset = BMPReader::readIntLE(inFile);     // offset to image
	BMPReader::readIntLE(inFile);              // Header size
	numCols = BMPReader::readIntLE(inFile);    // Number of columns in image (width)
	numRows = BMPReader::readIntLE(inFile);    // Number of rows in image (height)

	BMPReader::readShortLE(inFile);            // Should always be 1
	bpp = BMPReader::readShortLE(inFile);      // Bits per pixel, we support 24 or 32

	BMPReader::readIntLE(inFile);              // Compression method
	BMPReader::readIntLE(inFile);              // total size of bitmap data

	if (bpp != 24 && bpp != 32) {
		printf("Error: %s is not a 32 or 24 bits per pixel image.\n", filename);
		inFile.close();
		return NULL;
	}

	// If the image height (numRows) is negative, rows are stored top to
	// bottom, so we need to flip the image vertically.
	bool flipImage = false;
	if( numRows < 0 ) {
		flipImage = true;
		numRows = -numRows;
	}

	printf("Loading %s (%d bpp): %d x %d %d bytes (%s order).\n", filename, bpp, numCols,
			numRows, fileSize, flipImage ? "top to bottom" : "bottom to top");

	// add bytes at end of each row so total # is a multiple of 4
	// round up 3 * numCols to next mult. of 4
	int numPadBytes = 0, nBytesInRow;
	nBytesInRow = (bpp * numCols)/ 8;
	if( nBytesInRow % 4 != 0 ) numPadBytes = 4 - nBytesInRow % 4;

	height = numRows;
	width = numCols;
	GLubyte * pixelData = new GLubyte[width*height*4];
	if (!pixelData) {
		printf("Error: out of memory!\n");
		inFile.close();
		return NULL;
	}

	// Skip to the beginning of the pixel data
	inFile.seekg(offset);

	// Load pixels
	char pixBytes[4];
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			if( bpp == 32 )
				inFile.read(pixBytes,4);
			else {
				inFile.read(pixBytes,3);
				pixBytes[3] = char(0xff);     // set alpha to 255
			}

			int rowOut = row;
			if( flipImage ) rowOut = numRows - 1- row;

			int idx = ((rowOut * numCols) + col) * 4;
			pixelData[idx] = pixBytes[2]; // Red
			pixelData[idx+1] = pixBytes[1]; // Green
			pixelData[idx+2] = pixBytes[0]; // Blue
			pixelData[idx+3] = pixBytes[3]; // Alpha
		}
		// skip pad bytes at row end
		inFile.read(pixBytes, numPadBytes);
	}

	inFile.close();
	return pixelData;
}

GLuint BMPReader::loadTex(const char* fName, GLuint & width, GLuint &height) {

	GLubyte * data = BMPReader::load(fName, width, height);

	if( data != NULL ) {
		GLuint texID;
		glGenTextures(1, &texID);

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		delete [] data;

		return texID;
	}

	return 0;
}

GLuint BMPReader::loadTex(const char* fName) {
	GLuint w,h;
	return BMPReader::loadTex(fName, w, h);
}
