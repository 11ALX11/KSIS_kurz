#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>

#include "BMPCreation.h"
#include "BMPio.h"
#include "ImgBWFilters.h"

int main()
{
	// Create 'test.bmp' with white&black colors
//	BMPCreation* BMPCreatelib = new BMPCreation(); // new BMPCreation(5000, 5000);
//	BMPCreatelib->makeBlackDiagonalLine24bit();

	// Create BMPio for in/out methods.
	BMPio* bmp_io = new BMPio();

	// Init variables.
	std::string filename = "test512_mono.bmp"; // "BDL5000_monochrome.bmp";
	bool* pixels = nullptr;
	int pixels_height;
	int pixels_width;

	// Init timer.
	auto timer = std::chrono::steady_clock::now();

	// ToDo reading memory (supposed) issue.
	if (bmp_io->read(filename, pixels, pixels_height, pixels_width)) {
		cerr << "Error reading image!" << endl;
		return 1;
	}

	// Timer end and out.
	std::chrono::duration<double> time = std::chrono::steady_clock::now() - timer;
	cout << "Reading bmp: " << time.count() << "s" << endl;

	// Init filters.
	ImgBWFilters* filters = new ImgBWFilters();
	bool* pixels_new1 = nullptr;
	bool* pixels_new2 = nullptr;

	// Start timer.
	timer = std::chrono::steady_clock::now();

	// Apply median filter.
	if (filters->medianFilter(pixels, pixels_new1, pixels_height, pixels_width)) {
		cerr << "Error filtering(median) image!" << endl;
		return 3;
	}

	// Timer end and out.
	time = std::chrono::steady_clock::now() - timer;
	cout << "Filtering (median) bmp: " << time.count() << "s" << endl;

	// Start timer.
	timer = std::chrono::steady_clock::now();

	// Apply rotation filter.
	if (filters->rotate180degFilter(pixels_new1, pixels_new2, pixels_height, pixels_width)) {
		cerr << "Error filtering(rotate180deg) image!" << endl;
		return 3;
	}

	// Timer end and out.
	time = std::chrono::steady_clock::now() - timer;
	cout << "Filtering (rotate180deg) bmp: " << time.count() << "s" << endl;

	// Start timer.
	timer = std::chrono::steady_clock::now();

	// Save results.
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
	// Timer end and out.
	time = std::chrono::steady_clock::now() - timer;
	cout << "Saving bmps: " << time.count() << "s" << endl;

	/* Testing
	for (int i = 0; i < pixels_height; i++) {
		cout << i << " :";
		for (int j = 0; j < pixels_width; j++) {
			cout << ((pixels[i * pixels_width + j]) ? 'w' : 'b');
		}
		cout << endl;
	}

	// */
	return 0;
}