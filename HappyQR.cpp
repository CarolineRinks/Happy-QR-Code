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
		void write_jgraph(QR &, char *);

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

void QR::write_jgraph(QR &qr_code, char * fname) {
	
	Pixel pix;
	float r, g, b;
	int xtl, xtr, xbl, xbr;		// x values for each of the corners of square
	int ytl, ytr, ybl, ybr;		// y values for each of the corners of sqaure
	int count = 0;

	printf(newgraph.c_str());
	printf("xaxis nodraw\nyaxis nodraw\n");		// remove axes from jgraph

	/* Initialize y values for the corners of the starting pixel */
	ytl = qr_code.height;
	ytr = qr_code.height;
	ybl = ytl-1;	// 1 below
	ybr = ytr-1;	// 1 below

	for (int row = 0; row < qr_code.height; row++) {
		/* For each row, draw the leftmost pixel 1st */
		xtl = 0;
		xbl = 0;
		xtr = 1;
		xbr = 1;
		for (int col = 0; col < qr_code.width; col++) {
			/* Get pixel color value */
			pix = qr_code.grid[count];
			r = pix.R / 255.0;
			g = pix.G / 255.0;
			b = pix.B / 255.0;

			/* Print the current pixel by drawing a square and filling it with the pixel's color value */
			printf("newline poly pcfill %f %f %f\n\t", r, g, b);
			printf("pts %d %d  %d %d  %d %d  %d %d\n", xtl, ytl, xbl, ybl, xbr, ybr, xtr, ytr);
			
			/* Shift corners right to get the next pixel*/
			xtl += 1;
			xtr += 1;
			xbl += 1;
			xbr += 1;
			count += 1;
		}

		/* Shift corners down to get next row of pixels */
		ytl -= 1;
		ytr -= 1;
		ybl -= 1;
		ybr -= 1;
	}
}

void make_fun_qr(QR &qr_code) {
	//if have time show QR code for "happy" or a smiley face on program startup
	//cin << "Give me a QR code, and I'll make it happy!!! :)))))\n\n";
	//change background color
	//change random pixels to other colors
	//change random pixels to pattern pixels
	//change random pixels to pictures
	//test and see if certain permutations break the readability of qr
	//if needed, define constraints to the specified colors
	
	//could just have user specify 5 different colors, then it automatically picks the lightest one as the background

	/* Ask for user input */
/*
	cin << "Specify the colors for your QR code! You may either input one of the sample colors or provide your own hex color code in the form '#cccccc'.\nBackground Color:";

	vector<string> colors(0, 5);

	cin << "Specify up to 5 other colors that are darker than your background color\n";
	while (cin << 

	// Background color
	// Note: Dots should be 70% darker than the 
*/
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
	make_fun_qr(qr_code);
	qr_code.write_jgraph(qr_code, fname);

	return 0;
}

