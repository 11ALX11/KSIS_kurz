#include "ImgBWFilters.h"

bool ImgBWFilters::medianFilterFindMedian(int col_x, int row_y, bool*& pixels, int height, int width) {
	int white = 0;
	int all = 0;

	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			int x = col_x + i;
			int y = row_y + j;
			if (x >= 0 && x < width && y >= 0 && y < height) {
				all++;
				if (pixels[y * width + x]) white++;
			}
		}
	}

	if (white == all - white) return pixels[row_y * width + col_x]; // Don't change
	if (white > all - white) return true; // white
	return false; // black
}

int ImgBWFilters::medianFilter(bool*& pixels_old, bool*& pixels_new, int height, int width) {
	if (!pixels_old || height <= 0 || width <= 0) {
		return -1; // Invalid input parameters
	}

	// Allocate memory for new pixels
	pixels_new = new bool[height * width];

	// Apply filter
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			pixels_new[row * width + col] = medianFilterFindMedian(col, row, pixels_old, height, width);
		}
	}

	return 0; // Success
}

int ImgBWFilters::rotate180degFilter(bool*& pixels_old, bool*& pixels_new, int height, int width) {
	if (!pixels_old || height <= 0 || width <= 0) {
		return -1; // Invalid input parameters
	}

	// Allocate memory for the new array
	pixels_new = new bool[height * width];

	// Rotate the old array by 180 degrees and store the result in the new array
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			pixels_new[(height - 1 - i) * width + (width - 1 - j)] = pixels_old[i * width + j];
		}
	}

	return 0; // Success
}