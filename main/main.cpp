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
	std::string filename = "Garfield.bmp";
	// std::string filename = "BDL5000_monochrome.bmp";
	// std::string filename = "test64_mono.bmp";
	bool* pixels = nullptr;
	int pixels_height;
	int pixels_width;

	// Consistently
	// ***********************************************************************************
	cout << "Consistent:\n\n";

	// Init timer.
	auto timer = std::chrono::steady_clock::now();

	// ToDo reading memory issue.
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
	auto cons_timer = timer;

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

	std::chrono::duration<double> cons_time = std::chrono::steady_clock::now() - cons_timer;
	cout << "Consistent filtering bmp: " << cons_time.count() << "s" << endl;

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

	// Parallel
	// ******************************************************************************************
	cout << "\nParallel\n\n";

	pixels = nullptr;

	// Init timer.
	timer = std::chrono::steady_clock::now();

	// ToDo reading memory issue.
	if (bmp_io->read(filename, pixels, pixels_height, pixels_width)) {
		cerr << "Error reading image!" << endl;
		return 1;
	}

	// Timer end and out.
	time = std::chrono::steady_clock::now() - timer;
	cout << "Reading bmp: " << time.count() << "s" << endl;

	pixels_new1 = nullptr;
	pixels_new2 = nullptr;

	// Start timer.
	timer = std::chrono::steady_clock::now();
	auto paral_timer = timer;

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

	std::chrono::duration<double> paral_time = std::chrono::steady_clock::now() - paral_timer;
	cout << "Parallel filtering bmp: " << paral_time.count() << "s" << endl;

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

	// End, main section again

	cout << "\nEffectiveness: " << cons_time / paral_time << endl;
	return 0;
}