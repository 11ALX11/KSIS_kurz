#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "rgb_color.h"

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

class BMPReader
{

protected:

	vector<char> buffer;
	PBITMAPFILEHEADER file_header;
	PBITMAPINFOHEADER info_header;

public:

	// Puts image pixel's b, g, r into pixels(matrix) elements(rgb_color);
	// height and width - height and width of the matrix and image from image header.
	int read(string filename, rgb_color**& pixels, int& height, int& width);
};

