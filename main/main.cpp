#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "BMPCreation.h"
#include "BMPReader.h"
#include "rgb_color.h"

int main()
{
	std::cout << "Hello World!\n";

	// create 'test.bmp' with white&black colors
//  BMPCreation* BMPCreatelib = new BMPCreation(); // new BMPCreation(5000, 5000);
//  BMPCreatelib->makeBlackDiagonalLineWithWhiteNoise();

	std::string filename = "test.bmp";

	rgb_color** pixels = nullptr;
	int pixels_height;
	int pixels_width;

	BMPReader* reader = new BMPReader();
	if (reader->read(filename, pixels, pixels_height, pixels_width)) {
		cerr << "Error opening file!" << endl;
		return 1;
	}

	// Seems to be good, but test reading with writing

//	for (int i = 0; i < pixels_width; i++) {
//		cout << int(pixels[0][i].B) << " ";
//	}
//	cout << endl;

	return 0;
}