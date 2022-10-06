/* Caroline Rinks
 * CS494 Fall 2022
 * Lab #1 (Jgraph)
 *
 * This program reads a ppm file of a QR Code. It then decorates each of the
 * pixels (changing color of pixels, replacing pixels with images, and adding
 * patterns to pixels) and rewrites the updated QR code using jgraph.
*/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <random>
#include <unordered_map>
#include <fstream>

using namespace std;


struct Pixel {
	Pixel() { R = 0x00; G = 0x00; B = 0x00; eps = false; pattern = false; special = false;}
	void set(char r, char g, char b) { R = r; G = g; B = b; }

	unsigned char R, G, B;
	bool eps, pattern, special;
};

class QR {

	public:
		QR() { width = 0; height = 0; square_sz = 0; max_color = 0; }
		
		void read(const char *);
		void write_jgraph(const char *, const string &);

		void changeBackground();
		void changeColors();
		void changePatterns();
		void set_squares(Pixel &, int, int, int, int, int);
		string insertImages();

	private:
		int width;
		int height;
		int square_sz;
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

	fin.get(c);		// discard extra byte at beginning of qr code
	fin.get(c);
	fin.get(c);

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
			
			/* Each pixel is 2 RGBs, so just read a 2nd time */
			fin.get(c);
			fin.get(c);
			fin.get(c);
		}
	}
	if (count != width*height || (!fin.eof())) {
		cerr << "Reading error: " << count << " / " << width*height << " pixels read.\nfin.eof() -> " << fin.eof() << '\n';
		exit(EXIT_FAILURE);
	}
	fin.close();

	/* Set the length of the corner squares */
    int length = 0;
	for (int i = 1; i <= width; i++) {
        if (grid[i].R != 0x00) {
            break;
        }
        length += 1;
    }
	square_sz = length;
}

void QR::write_jgraph(const char * fname, const string &eps_img) {
	
	ofstream fout(fname);
	Pixel pix;
	float r, g, b;
	int xtl, xtr, xbl, xbr;		// x values for each of the corners of square
	int ytl, ytr, ybl, ybr;		// y values for each of the corners of sqaure
	int count = 1;

	/* Begin writing jgraph file */
	fout << "newgraph\n";
	fout << "xaxis nodraw\nyaxis nodraw\n";		// remove axes from jgraph

	/* Initialize y values for the corners of the starting pixel */
	ytl = height;
	ytr = height;
	ybl = height-1;
	ybr = height-1;

	for (int row = 0; row < height; row++) {
		/* For each row, draw the leftmost pixel 1st */
		xtl = 0;
		xbl = 0;
		xtr = 1;
		xbr = 1;
		for (int col = 0; col < width; col++) {
			
			/* If pixel is marked for replacement by image, draw the eps.
			 * Otherwise, draw a filled square */
			if (grid[count].eps == true) {
				fout << "newcurve eps " << eps_img << " marksize 1 1\n\t";
				fout << "pts " << xbl+0.5 << " " << ybl+0.5 << '\n';
			}
			else {
				/* Get pixel color value */
				pix = grid[count];
				r = pix.R / 255.0;
				g = pix.G / 255.0;
				b = pix.B / 255.0;

				fout << "newline poly pcfill " 
					 << r << " " << g << " " << b << " color "
					 << r << " " << g << " " << b;

				/* If pixel pattern attribute is marked, change the pattern */
				if (pix.pattern == true) {	
					fout << " ppattern stripe 60";
				}
				fout << "\n\tpts "
					 << xtl << " " << ytl << "  " 
					 << xbl << " " << ybl << "  " 
					 << xbr << " " << ybr << "  "
					 << xtr << " " << ytr << '\n';

			}
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
	fout.close();
}

void QR::set_squares(Pixel &pix, int n, int itl, int itr, int ibl, int ibr) {
	
	for (int i = 0; i < n-1; i++) {
		grid[itl].set(pix.R, pix.G, pix.B);
		grid[itl].special = true;
		
		grid[itr].set(pix.R, pix.G, pix.B);
		grid[itr].special = true;
		
		grid[ibl].set(pix.R, pix.G, pix.B);
		grid[ibl].special = true;
		
		grid[ibr].set(pix.R, pix.G, pix.B);
		grid[ibr].special = true;

		/* Shift pixels (counterclockwise) */
		itl += width;
		ibl += 1;
		ibr -= width;
		itr -= 1;
	}
}

void QR::changeColors() {
	
	string r, g, b;
	Pixel pix;
	vector<Pixel> colors;

	/* Ask for user input */
	cout << "CHOOSE YOUR COLORS!\n. . . Specify up to 5 colors each inputed as a set of RGB values.\n. . . For example, input '255 0 0' for the color red.\n. . . When you are finished, input 'done'.\n\n";
	
	while (colors.size() <= 5) {
		cout << ":> ";

		/* Stop reading when 10 colors have been input or user inputs 'done' */
		cin >> r;
		if (r == "done") { break; }
		else if (stoi(r) > 255 || stoi(r) < 0) {
			cout << "Invalid color! Each R G B value should be in the range 0-255.\n";
			cin.clear();
			fflush(stdin);
			continue;
		}

		cin >> g;
		if (g == "done") { break; }
		else if (stoi(g) > 255 || stoi(g) < 0) {
            cout << "Invalid color! Each R G B value should be in the range 0-255.\n";
            cin.clear();
            fflush(stdin);
            continue;
        }
		
		cin >> b;
		if (b == "done") { break; }
		else if (stoi(b) > 255 || stoi(b) < 0) {
            cout << "Invalid color! Each R G B value should be in the range 0-255.\n";
            cin.clear();
            fflush(stdin);
            continue;
        }

		/* Add color to list of colors */
		pix.R = stoi(r);
		pix.G = stoi(g);
		pix.B = stoi(b);
		colors.push_back(pix);
	}

	//cin.clear();
	//fflush(stdin);

	if (colors.size() != 0) {

		/* Set up random number generator */
		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distr(0, colors.size()-1);

		/* Change the color of each of the dark pixels to one of the colors
		 * given by the user. */
		int count = 1;
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {

				/* Skip white pixels */
				if (grid[count].R == 0xff) { 
					count += 1; 
					continue; 
				}

				/* Randomly choose one of the given colors and set the current
				 * pixel to that color. */
				pix = colors.at(distr(generator));
				grid[count].R = pix.R;
				grid[count].G = pix.G;
				grid[count].B = pix.B;

				count += 1;
			}
		}

		/* Adjust the corner squares so that they are a single solid fill.
		 * This helps make the QR code more readable. */

		/* Top Left square (Outer) */
		pix = colors.at(distr(generator));
		set_squares(pix, square_sz, 1, square_sz, (square_sz-1)*width+1, (square_sz-1)*width+square_sz);
		/* Top Right square (Outer) */
		pix = colors.at(distr(generator));
		set_squares(pix, square_sz, width-square_sz+1, width, width*square_sz-square_sz+1, width*square_sz);
		/* Bottom Left square (Outer) */
		pix = colors.at(distr(generator));
		set_squares(pix, square_sz, width*(height-square_sz)+1, width*(height-square_sz)+square_sz, height*width-width+1, height*width-width+square_sz);

		int itl, itr, mid;

		/* Top Left square (inner) */
		itl = (1+width)*2 + 1;
		pix = colors.at(distr(generator));
		set_squares(pix, square_sz-4, itl, itl+2, itl+width+width, itl+width+width+2);
		mid = itl + width + 1;
		grid[mid].R = pix.R;
		grid[mid].G = pix.G;
		grid[mid].B = pix.B;

		/* Top Right square (inner) */
        itr = width*3-2;
        pix = colors.at(distr(generator));
        set_squares(pix, square_sz-4, itr-2, itr, itr-2+width+width, itr+width+width);
        mid = itr + width - 1;
        grid[mid].R = pix.R;
        grid[mid].G = pix.G;
        grid[mid].B = pix.B;

		/* Bottom left square (inner) */
		itl = (width*(height-square_sz)+1) + (width+1)*2;
        pix = colors.at(distr(generator));
        set_squares(pix, square_sz-4, itl, itl+2, itl+width+width, itl+width+width+2);
        mid = itl + width + 1;
        grid[mid].R = pix.R;
        grid[mid].G = pix.G;
        grid[mid].B = pix.B;
	}
	else{
		cout << "\nNo colors given. Your QR code will be in standard Black and White\n";
	}
}

void QR::changePatterns() {
	
	int num;
	string input;
	
	cout << "\nWould you like some of the pixels to be striped? (yes/no)\n:> ";
	cin >> input;
	
	if (input.compare("yes") == 0) {

		/* For a random set of pixels, indicate in the grid that
		 * the pixel should be striped */
		random_device rd;
		mt19937 generator(rd());
		uniform_int_distribution<int> distr(1, width*height);
		
		for (int i = 0; i < 10; i++) {
			num = distr(generator);
			if (grid[num].special == true) {
				// if pixel is one of the corner squares, generate a different pixel
				i -= 1;
				continue;
			}
			grid[num].pattern = true;
		}
	}
}

string QR::insertImages() {
	
	int num;
	string file;

	/* Specify an eps image to replace pixels with */
	cout << "\nChoose an image! Input the name of an eps image.\n:> ";
	while (cin >> file) {
		
		/* Check image is valid */
		if (strstr(file.c_str(), ".eps") == NULL) {
			cout << "That file is not the correct type. Please specify a valid .eps file.\n:> ";
		}
		else{
			ifstream fin(file);
			if (!fin.is_open()) {
				cout << "That file does not exist. Please specify a valid .eps file.\n:> ";
			}
			else{
				fin.close();
				break;
			}
		}
	}

	/* For a random set of pixels, indicate in the grid that 
	 * the pixel should be replaced by the specified image */
	random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distr(1, width*height);
	
	for (int i = 0; i < 12; i++) {
		num = distr(generator);
		if (grid[num].R == 0xff) {
			i -= 1;			// skip white pixels
			continue;
		}
		else if (grid[num].special == true) {
			// If pixel is one of the corner squares, generate a different pixel
			i -= 1;
			continue;
		}
		grid[num].eps = true;
	}
	return file;
} 
	
int main(int argc, char *argv[]) {

	QR qr_code;
	char * fname_in, * fname_out;

	if (argc != 3) {
		cerr << "usage: ./HappyQR file.ppm file.jgr\n";
		return -1;
	}	
	fname_in = argv[1];
	fname_out = argv[2];	
	qr_code.read(fname_in);
	
	/* Make the QR Code Happy */
	qr_code.changeColors();
	string eps_img = qr_code.insertImages();
	qr_code.changePatterns();
	qr_code.write_jgraph(fname_out, eps_img);

	return 0;
}

