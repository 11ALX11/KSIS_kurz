#pragma once

#ifndef BI_RGB
#define BI_RGB 0
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct tagBITMAPFILEHEADER {
	WORD  fhType;
	DWORD fhSize;
	WORD  fhReserved1;
	WORD  fhReserved2;
	DWORD fhOffBits;
} BITMAPFILEHEADER, * PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD ihSize;
	LONG  ihWidth;
	LONG  ihHeight;
	WORD  ihPlanes;
	WORD  ihBitCount;
	DWORD ihCompression;
	DWORD ihSizeImage;
	LONG  ihXPelsPerMeter;
	LONG  ihYPelsPerMeter;
	DWORD ihClrUsed;
	DWORD ihClrImportant;
} BITMAPINFOHEADER, * PBITMAPINFOHEADER;

class BMPio
{

protected:

	vector<char> buffer;
	BITMAPFILEHEADER file_header;
	BITMAPINFOHEADER info_header;

public:

	// Puts image's pixels(bits) into pixels(matrix) elements(bool);
	// height and width - height and width of the matrix and image from image header.
	// Read ONLY white-black bmp images.
	// Return 0 if success.
	int read(string filename, bool**& pixels, int& height, int& width);

	// Writes out pixels(matrix) into bmp image 'filename'.
	// Use AFTER read, or fill header structure manually.
	// Write ONLY white-black bmp images.
	// Return 0 if success.
	int writeWithHeader(string filename, bool**& pixels, int& height, int& width);
};

