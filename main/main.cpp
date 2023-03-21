#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "BMPCreation.h"
#include "BMPio.h"

int main()
{
	// create 'test.bmp' with white&black colors
//	BMPCreation* BMPCreatelib = new BMPCreation(100, 100); // new BMPCreation(5000, 5000);
//	BMPCreatelib->makeBlackDiagonalLine24bit();

	// create BMPio for in/out methods
	BMPio* bmp_io = new BMPio();

	// init variables
	std::string filename = "test_mono.bmp";
	bool** pixels = nullptr;
	int pixels_height;
	int pixels_width;

	if (bmp_io->read(filename, pixels, pixels_height, pixels_width)) {
		cerr << "Error reading image!" << endl;
		return 1;
	}

	if (bmp_io->writeWithHeader("test_out.bmp", pixels, pixels_height, pixels_width)) {
		cerr << "Error saving image!" << endl;
		return 2;
	}

	/* Testing
	for (int i = 0; i < pixels_height; i++) {
		cout << i << " :";
		for (int j = 0; j < pixels_width; j++) {
			cout << ((pixels[i][j]) ? 'w' : 'b');
		}
		cout << endl;
	}

	// */
	return 0;
}