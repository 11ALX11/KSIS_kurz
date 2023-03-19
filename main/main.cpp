#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "BMPCreation.h"
#include "BMPio.h"

int main()
{
	std::cout << "Hello World!\n";

	// create 'test.bmp' with white&black colors
//	BMPCreation* BMPCreatelib = new BMPCreation(); // new BMPCreation(5000, 5000);
//	BMPCreatelib->makeBlackDiagonalLineWithWhiteNoise1bit();

	std::string filename = "test64_mono.bmp";

	bool** pixels = nullptr;
	int pixels_height;
	int pixels_width;

	BMPio* reader = new BMPio();
	if (reader->read(filename, pixels, pixels_height, pixels_width)) {
		cerr << "Error opening file!" << endl;
		return 1;
	}



	// Testing
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