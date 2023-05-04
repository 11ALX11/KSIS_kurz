#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>

#include "BMPCreation.h"
#include "BMPio.h"
#include "ImgBWFilters.h"

#include "mpi.h"

int main(int argc, char** argv) {

	// Create 'test.bmp' with white&black colors
	//	BMPCreation* BMPCreatelib = new BMPCreation(); // new BMPCreation(5000, 5000);
	//	BMPCreatelib->makeBlackDiagonalLine24bit();

	// Create BMPio for in/out methods.
	BMPio* bmp_io = new BMPio();
	// Init filters.
	ImgBWFilters* filters = new ImgBWFilters();

	// Init variables.
	// std::string filename = "Garfield.bmp";
	// std::string filename = "BDL5000_monochrome.bmp";
	std::string filename = "test64_mono.bmp";
	bool* pixels = nullptr;
	bool* pixels_new1 = nullptr;
	bool* pixels_new2 = nullptr;
	int pixels_height;
	int pixels_width;


	// Parallel
	// ******************************************************************************************


	// Init timer.
	auto timer = std::chrono::steady_clock::now();

	int thread, thread_size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &thread);
	MPI_Comm_size(MPI_COMM_WORLD, &thread_size);

	// Timer end and out.
	std::chrono::duration<double> time_mpi_init = std::chrono::steady_clock::now() - timer;

	// if not first thread (not parent)
	if (thread != 0) {

		// Parallel thread (worker)

		int pixels_height, pixels_width;

		MPI_Status status;
		MPI_Recv(&pixels_height, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(&pixels_width, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		// Allocate memory for the pixels.
		pixels = new bool[pixels_height * pixels_width];

		MPI_Recv(pixels, pixels_height * pixels_width, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		// Apply median filter.
		if (filters->medianFilter(pixels, pixels_new1, pixels_height, pixels_width)) {
			cerr << "Error filtering(median) image!" << endl;
			return 3;
		}

		// Apply rotation filter.
		if (filters->rotate180degFilter(pixels_new1, pixels_new2, pixels_height, pixels_width)) {
			cerr << "Error filtering(rotate180deg) image!" << endl;
			return 3;
		}

		MPI_Send(pixels_new2, pixels_height * pixels_width, MPI_BYTE, 0, 0, MPI_COMM_WORLD);

		MPI_Finalize();
	}
	else {
		// Main thread (parent)
		cout << "\nParallel, with " << thread_size << " thread(s).\n\n";

		cout << "Init mpi (parent thread): " << time_mpi_init.count() << "s" << endl;

		if (bmp_io->read(filename, pixels, pixels_height, pixels_width)) {
			cerr << "Error reading image!" << endl;
			return 1;
		}

		// Start timer.
		auto timer = std::chrono::steady_clock::now();
		auto paral_timer = timer;

		// Send data to other threads
		int new_height = pixels_height / thread_size;
		int buff_size = pixels_width * new_height;
		for (int to_thread = 1; to_thread < thread_size; to_thread++) {
			MPI_Send(&new_height, 1, MPI_INT, to_thread, 0, MPI_COMM_WORLD);
			MPI_Send(&pixels_width, 1, MPI_INT, to_thread, 0, MPI_COMM_WORLD);
			MPI_Send(pixels + to_thread * buff_size, buff_size, MPI_BYTE, to_thread, 0, MPI_COMM_WORLD);
		}

		// Apply median filter.
		if (filters->medianFilter(pixels, pixels_new1, new_height, pixels_width)) {
			cerr << "Error filtering(median) image!" << endl;
			return 3;
		}

		// Apply rotation filter.
		if (filters->rotate180degFilter(pixels_new1, pixels_new2, new_height, pixels_width)) {
			cerr << "Error filtering(rotate180deg) image!" << endl;
			return 3;
		}

		// Copy pixels_new2 into pixels
		for (int i = 0; i < new_height; i++) {
			for (int j = 0; j < pixels_width; j++) {
				pixels[(pixels_height - new_height + i) * pixels_width + j] = pixels_new2[i * pixels_width + j];
			}
		}

		// Retrieve data from threads and assemble
		MPI_Status status;
		for (int to_thread = 1; to_thread < thread_size; to_thread++) {
			MPI_Recv(pixels + buff_size * (thread_size - to_thread - 1), buff_size, MPI_BYTE, to_thread, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		}

		std::chrono::duration<double> paral_time = std::chrono::steady_clock::now() - paral_timer + time_mpi_init;
		cout << "Parallel filtering bmp (with mpi init): " << paral_time.count() << "s" << endl;

		// Save results.
		if (bmp_io->writeWithHeader("test3_out.bmp", pixels, pixels_height, pixels_width)) {
			cerr << "Error saving image!" << endl;
			return 2;
		}

		MPI_Finalize();


		// Consistently
		// ***********************************************************************************
		cout << "\nConsistent:\n\n";


		if (bmp_io->read(filename, pixels, pixels_height, pixels_width)) {
			cerr << "Error reading image!" << endl;
			return 1;
		}

		// Start timer.
		timer = std::chrono::steady_clock::now();
		auto cons_timer = timer;

		// Apply median filter.
		if (filters->medianFilter(pixels, pixels_new1, pixels_height, pixels_width)) {
			cerr << "Error filtering(median) image!" << endl;
			return 3;
		}

		// Timer end and out.
		std::chrono::duration<double> time = std::chrono::steady_clock::now() - timer;
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

		cout << "\nEffectiveness: " << cons_time / paral_time << endl;
	}

	return 0;
}