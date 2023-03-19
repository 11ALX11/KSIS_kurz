#pragma pack(1)

#include "BMPio.h"

int BMPio::read(string filename, bool**& pixels, int& height, int& width) {

	std::ifstream file(filename);

	if (file) {
		file.seekg(0, std::ios::end);
		streampos length = file.tellg();
		file.seekg(0, std::ios::beg);

		buffer.resize(length);
		file.read(&buffer[0], length);

		file_header = (PBITMAPFILEHEADER)(&buffer[0]);
		info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));

		height = info_header->ihHeight;
		width = info_header->ihWidth;

		pixels = new bool * [height];
		for (int i = 0; i < height; i++) {
			pixels[i] = new bool[width];
		}

		// Calculate the row padding (in bytes) of the bmp image.
		int row_padding = (4 - ((width / 8) % 4)) % 4;

		// The pixel data begins after the BITMAPFILEHEADER and BITMAPINFOHEADER.
		unsigned char* pixel_data = (unsigned char*)&buffer[0] + file_header->fhOffBits;

		// Iterate over each row of the pixel data.
		for (int row = 0; row < height; row++) {

			// Calculate the starting byte index of the current row.
			int row_start_index = row * ((width / 8) + row_padding);

			// Iterate over each column of the current row.
			for (int col = 0; col < width; col++) {

				// Calculate the byte index and bit index of the current pixel.
				int byte_index = row_start_index + col / 8;
				int bit_index = col % 8;

				// Extract the pixel's bit value from the byte.
				bool pixel_value;
				if (info_header->ihCompression == BI_RGB) {  // uncompressed format
					// Little-endian bit order.
					// in theory
					// pixel_value = (pixel_data[byte_index] & (1 << bit_index)) != 0;

					// in practice
					pixel_value = (pixel_data[byte_index] & (0x80 >> bit_index)) != 0;
				}
				else {  // compressed format (e.g., BI_BITFIELDS, BI_ALPHABITFIELDS)
					// Big-endian bit order.
					// in theory
					// pixel_value = (pixel_data[byte_index] & (0x80 >> bit_index)) != 0;

					// in practice
					pixel_value = (pixel_data[byte_index] & (1 << bit_index)) != 0;
				}

				// Store the pixel value in the pixels 2D array.
				pixels[height-row-1][col] = pixel_value;
			}
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