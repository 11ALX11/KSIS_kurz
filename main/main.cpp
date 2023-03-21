#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "BMPCreation.h"
#include "BMPio.h"
#include "ImgBWFilters.h"

int main()
{
	// create 'test.bmp' with white&black colors
//	BMPCreation* BMPCreatelib = new BMPCreation(); // new BMPCreation(5000, 5000);
//	BMPCreatelib->makeBlackDiagonalLine24bit();

	// create BMPio for in/out methods
	BMPio* bmp_io = new BMPio();

	// init variables
	std::string filename = "test512_mono.bmp";
	bool* pixels = nullptr;
	int pixels_height;
	int pixels_width;

	// ToDo reading memory (supposed) issue.
	if (bmp_io->read(filename, pixels, pixels_height, pixels_width)) {
		cerr << "Error reading image!" << endl;
		return 1;
	}

	ImgBWFilters* filters = new ImgBWFilters();
	bool* pixels_new1 = nullptr;
	bool* pixels_new2 = nullptr;

	if (filters->medianFilter(pixels, pixels_new1, pixels_height, pixels_width)) {
		cerr << "Error filtering(median) image!" << endl;
		return 3;
	}
	if (filters->rotate180degFilter(pixels, pixels_new2, pixels_height, pixels_width)) {
		cerr << "Error filtering(rotate180deg) image!" << endl;
		return 3;
	}

	if (bmp_io->writeWithHeader("test1_out.bmp", pixels, pixels_height, pixels_width)) {
		cerr << "Error saving image!" << endl;
		return 2;
	}
	if (bmp_io->writeWithHeader("test2_out.bmp", pixels_new1, pixels_height, pixels_width)) {
		cerr << "Error saving image!" << endl;
		return 2;
	}
	if (bmp_io->writeWithHeader("test3_out.bmp", pixels_new2, pixels_height, pixels_width)) {
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