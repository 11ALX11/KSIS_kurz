#pragma pack(1)

#include "BMPio.h"

int BMPio::read(string filename, bool*& pixels, int& height, int& width) {

	std::ifstream file(filename);

	if (file) {
		// Read the file header.
		file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));

		// Read the info header.
		file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

		// Read colors after header
		ColorsAfterHeader cah;
		file.read(reinterpret_cast<char*>(&cah), sizeof(ColorsAfterHeader));

		// Check that the file is a 1-bit BMP image.
		if (info_header.ihBitCount != 1) {
			return -1;
		}

		// Set height and width
		height = info_header.ihHeight;
		width = info_header.ihWidth;

		// Calculate the row size in bytes.
		int row_size = (width + 7) / 8;

		// Calculate the padding size in bytes.
		int padding_size = (4 - row_size % 4) % 4;

		// Allocate memory for the pixels.
		pixels = new bool[height * width];

		// Init row data 
		char* row_data = new char[row_size];

		// Read the pixels row by row.
		for (int row = height - 1; row >= 0; row--) {

			// Read the row of pixels.
			file.read(row_data, row_size);

			// Iterate over each column of the current row.
			for (int col = 0; col < width; col++) {
				// Calculate the byte index and bit index of the current pixel.
				int byte_index = col / 8;
				int bit_index = col % 8;

				// Extract the pixel's bit value from the byte.
				bool pixel_value;
				if (info_header.ihCompression == BI_RGB) {  // uncompressed format
					// Little-endian bit order.
					// in theory
					// pixel_value = (row_data[byte_index] & (1 << bit_index)) != 0;

					// in practice
					pixel_value = (row_data[byte_index] & (0x80 >> bit_index)) != 0;
				}
				else {  // compressed format (e.g., BI_BITFIELDS, BI_ALPHABITFIELDS)
					// Big-endian bit order.
					// in theory
					// pixel_value = (row_data[byte_index] & (0x80 >> bit_index)) != 0;

					// in practice
					pixel_value = (row_data[byte_index] & (1 << bit_index)) != 0;
				}

				// Store the pixel value in the pixels 2D array.
				pixels[row * width + col] = pixel_value;
			}

			// Skip the row padding bytes.
			file.seekg(padding_size, std::ios::cur);
		}

		// Free the row data memory.
		delete[] row_data;
	}
	else {
		return -1;
	}

	return 0;
}

int BMPio::writeWithHeader(string filename, bool*& pixels, int& height, int& width) {

	// Calculate the row size in bytes.
	int row_size = (width + 7) / 8;

	// Calculate the padding size in bytes.
	int padding_size = (4 - row_size % 4) % 4;

	// Calculate the image size in bytes.
	int image_size = (row_size + padding_size) * height;

	// Open the output file for writing.
	std::ofstream file(filename, std::ios::binary);

	// Check if file opened/created successfully
	if (!file) {
		return -1;
	}

	// Set the file header fields.
	file_header.fhType = 0x4D42;
	file_header.fhSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(ColorsAfterHeader) + image_size;
	file_header.fhReserved1 = 0;
	file_header.fhReserved2 = 0;
	file_header.fhOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(ColorsAfterHeader);

	// Set the info header fields.
	info_header.ihSize = sizeof(BITMAPINFOHEADER);
	info_header.ihWidth = width;
	info_header.ihHeight = height;
	info_header.ihPlanes = 1;
	info_header.ihBitCount = 1;
	info_header.ihCompression = 0;
	info_header.ihSizeImage = image_size;
	info_header.ihXPelsPerMeter = 3780;
	info_header.ihYPelsPerMeter = 3780;
	info_header.ihClrUsed = 0;
	info_header.ihClrImportant = 0;

	// Write the file header.
	file.write(reinterpret_cast<const char*>(&file_header), sizeof(BITMAPFILEHEADER));

	// Write the info header.
	file.write(reinterpret_cast<const char*>(&info_header), sizeof(BITMAPINFOHEADER));

	// Write colors after header
	ColorsAfterHeader cah;
	file.write(reinterpret_cast<const char*>(&cah), sizeof(ColorsAfterHeader));

	// Write the pixels row by row.
	for (int row = height - 1; row >= 0; row--) {

		// Initialize the row data with zeros.
		char* row_data = new char[row_size];
		std::fill_n(row_data, row_size, 0);

		// Iterate over each column of the current row.
		for (int col = 0; col < width; col++) {
			// Calculate the byte index and bit index of the current pixel.
			int byte_index = col / 8;
			int bit_index = col % 8;

			// Set the pixel's bit value in the byte.
			if (pixels[row * width + col]) {
				row_data[byte_index] |= (0x80 >> bit_index);
			}
		}

		// Write the row of pixels.
		file.write(row_data, row_size);

		// Free the row data memory.
		delete[] row_data;

		// Write the row padding bytes.
		char* row_padding = new char[padding_size];
		std::fill_n(row_padding, padding_size, 0);
		file.write(row_padding, padding_size);
		delete[] row_padding;
	}

	// Close the output file.
	file.close();

	return 0;
}
