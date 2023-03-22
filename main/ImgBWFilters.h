#pragma once

#include <vector>
#include <algorithm>

using namespace std;

// Class for filtering black and white 1-bit per pixel images.
// Accepts 2D arrays of bool (ex. pixels[y * width + x]).
class ImgBWFilters
{
protected:

	// Method helper for MedianFilter
	// Returns median.
	bool medianFilterFindMedian(int col_x, int row_y, bool*& pixels, int height, int width);

public:

	// Median filter for black and white img (bool) 2D array.
	int medianFilter(bool*& pixels_old, bool*& pixels_new, int height, int width);

	// Rotate 2D array (bool) by 180deg.
	int rotate180degFilter(bool*& pixels_old, bool*& pixels_new, int height, int width);
};

