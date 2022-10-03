/* Caroline Rinks
 * CS494 Fall 2022
 * Lab #1 (Jgraph)
*/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "keywords.h"

using namespace std;


struct Pixel {
	Pixel() { R = 0; G = 0; B = 0; }
	void set(const int r, const int g, const int b) { R = r; G = g; B = b; }
	
	unsigned char R, G, B;
};

class QR {

	public:
		QR() { width = 0; height = 0; max_color = 0; }
		void read(const char *);

	private:
		int width;
		int height;
		int max_color;
		unordered_map<int, Pixel> grid;
};

void QR::read(const char *fname) {
	
	ifstream fin;
	string ID;

	fin.open(fname, ios::binary);
	if (!fin.is_open()) {
		perror(fname);
		exit(EXIT_FAILURE);
	}

	/* Read header info */
	fin >> ID;
	if (ID != "P6") {
		cerr << "QR Code PPM file must be of type P6\n";
		exit(EXIT_FAILURE);
	}

	fin >> width >> height >> max_color;
	while (fin.get() != '\n') {}

	Pixel pix;
	char c;
	int count = 0;

	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			count += 1;
			
			fin.get(c);
			pix.R = (unsigned) c;
			fin.get(c);
			pix.G = (unsigned) c;
			fin.get(c);
			pix.B = (unsigned) c;

			grid[count] = pix;
		}
	}
	
	if (count != width*height) {
		cerr << count << " / " << width*height << " pixels read.\n";
		exit(EXIT_FAILURE);
	}
/*
	for (int i = 0; i < width*height; i++) {
		printf("grid[%d] = %x %x %x\n", i, grid[i].R, grid[i].G, grid[i].B);
	}
*/
}


void write_jgraph() {
	
}

int main(int argc, char *argv[]) {

	char * fname;
	QR qr_code;

	if (argc != 2) {
		cerr << "args wrong\n";
		return -1;
	}
	
	fname = argv[1];
	qr_code.read(fname);

	printf(newgraph.c_str());

	return 0;
}

