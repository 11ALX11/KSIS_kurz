#pragma once

// Make a bitmap BMP image
//  - image size is defined by variables
//  - saves images into 'test.bmp'
class BMPCreation
{
protected:

	int height;
	int width;

	// fill 4 bytes in the header array given an int. 
	// bytes in a BMP file are written like this:
	// (16^1 + 16^0)   (16^3 + 16^2)   (16^5 + 16^4) (16^7 + 16^6)
	void fillHeaderVal(int val, int placeHigh, unsigned char* header);

public:

	BMPCreation();
	BMPCreation(int height, int width);

	// create bmp image of a black diagonal line on a white background
	void makeBlackDiagonalLine24bit();
};

