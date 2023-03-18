#pragma once

// make a bitmap BMP image
//   - this code produces and image of 5 rows going from blue at the bottom to red at the top
//   - image size is defined by variables
//	 - saves images into 'test.bmp'
//
// Author: Peter Sanchez
// Date: 9_4_2022
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

	// this code produces and image of 5 rows going from blue at the bottom to red at the top
	// height must be divisable by 5 since we are making 5 rows
	void makeRainbowLines();

	// create bmp image of a black diagonal line on a white background
	void makeBlackDiagonalLine();

	// create bmp image of a black diagonal line on a white background
	// with white noise 
	void makeBlackDiagonalLineWithWhiteNoise();
};

