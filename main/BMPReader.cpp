#pragma pack(1)

#include "BMPReader.h"

int BMPReader::read(string filename, rgb_color**& pixels, int& height, int& width) {

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

		pixels = new rgb_color * [height];
		for (int i = 0; i < height; i++) {
			pixels[i] = new rgb_color[width];
		}

		char* header_offset = &buffer[0] + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		int single_color_size = sizeof(uint8_t);
		int color_size = sizeof(rgb_color);
		int cnt = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				uint8_t* b = (uint8_t*)header_offset + color_size * cnt + single_color_size;
				uint8_t* g = (uint8_t*)header_offset + color_size * cnt + single_color_size * 2;
				uint8_t* r = (uint8_t*)header_offset + color_size * cnt++ + single_color_size * 3;
				pixels[i][j].B = *b;
				pixels[i][j].G = *g;
				pixels[i][j].R = *r;
			}
		}
	}
	else {
		return -1;
	}

	return 0;
}