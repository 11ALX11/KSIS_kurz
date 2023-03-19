#pragma pack(1)

#include "BMPio.h"

int BMPio::read(string filename, bool**& pixels, int& height, int& width) {

	std::ifstream file(filename);

	if (file) {
		// Read the file header.
		file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));

		// Read the info header.
		file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

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
		pixels = new bool* [height];
		for (int i = 0; i < height; i++) {
			pixels[i] = new bool[width];
		}

		// Calculate the row padding (in bytes) of the bmp image.
		int row_padding = (4 - ((width / 8) % 4)) % 4;

		// The pixel data begins after the BITMAPFILEHEADER and BITMAPINFOHEADER.
		unsigned char* pixel_data = (unsigned char*)&buffer[0] + file_header.fhOffBits;

		// Read the pixels row by row.
		for (int row = height; row >= 0; row--) {

			// Read the row of pixels.
			char* row_data = new char[row_size];
			file.read(row_data, row_size);

			// Skip the padding bytes.
			file.seekg(padding_size, std::ios::cur);

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
				if (row != height) pixels[row][col] = pixel_value;
			}

			// Free the row data memory.
			delete[] row_data;

			// Skip the row padding bytes.
			file.seekg(row_padding, std::ios::cur);
		}
	}
	else {
		return -1;
	}

	return 0;
}

int BMPio::modify(string filename, bool**& pixels, int& heigth, int& width) {
	//ToDo
	return 0;
}