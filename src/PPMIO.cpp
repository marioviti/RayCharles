
#include "PPMIO.h"

// Source courtesy of J. Manson
// http://josiahmanson.com/prose/optimize_ppm/


namespace PPMIO{
using namespace std;

void write_ppm(  vector<Vec3>& image, string&  filename, int w, int h) {
	ofstream f(filename.c_str(), ios::binary);
	if (f.fail()) {
			std::cout << "Could not open file: " << filename << std::endl;
			return;
	}
	// A "magic number" for identifying the file type. A Plain ppm image's magic number is the two characters "P3".
	f << "P3";
	//Whitespace (blanks, TABs, CRs, LFs).
	f << '\t';
	// A width, formatted as ASCII characters in decimal.
	f << w;
	// Whitespace.
	f << ' ';
	// A height, again in ASCII decimal.
	f << h;
	// newline.
	f << std::endl;
	// the maximum color value (Maxval), again in ASCII decimal. Must be less than 65536 and more than zero
	f << RGB_COMPONENT_COLOR;
	// A single whitespace character (usually a newline).
	f << std::endl;
	// A raster of Height rows, in order from top to bottom.
	// Each sample in the raster has white space before and after it.
	// There must be at least one character of white space between any two samples,
	// but there is no maximum. There is no particular separation of one pixel
	// from another -- just the required separation between the blue sample of
	// one pixel from the red sample of the next pixel.
	// The most significant byte is first.

	for (int y=0; y<h; y++){
		for (int x=0; x<w; x++) {
			f << ' ' << (int)(RGB_COMPONENT_COLOR*image[x + y*w][0]); // R component
			f << ' ' << (int)(RGB_COMPONENT_COLOR*image[x + y*w][1]); // G component
			f << ' ' << (int)(RGB_COMPONENT_COLOR*image[x + y*w][2]) << ' '; // Bcomponent
		}
		f << std::endl;
	}
	f.close();
}

void eat_comment(ifstream &f)
{
    char linebuf[1024];
    char ppp;
    while (ppp = f.peek(), ppp == '\n' || ppp == '\r')
        f.get();
    if (ppp == '#')
        f.getline(linebuf, 1023);
}

void load_ppm( unsigned char * & pixels , unsigned int & w , unsigned int & h , const string &name , loadedFormat format)
{
    ifstream f(name.c_str(), ios::binary);
    if (f.fail())
    {
        cout << "Could not open file: " << name << endl;
        return;
    }

    // get type of file
    eat_comment(f);
    int mode = 0;
    string s;
    f >> s;
    if (s == "P3")
        mode = 3;
    else if (s == "P6")
        mode = 6;

    // get w
    eat_comment(f);
    f >> w;

    // get h
    eat_comment(f);
    f >> h;

    // get bits
    eat_comment(f);
    int bits = 0;
    f >> bits;

    // error checking
    if (mode != 3 && mode != 6)
    {
        cout << "Unsupported magic number" << endl;
        f.close();
        return;
    }
    if (w < 1)
    {
        cout << "Unsupported width: " << w << endl;
        f.close();
        return;
    }
    if (h < 1)
    {
        cout << "Unsupported height: " << h << endl;
        f.close();
        return;
    }
    if (bits < 1 || bits > 255)
    {
        cout << "Unsupported number of bits: " << bits << endl;
        f.close();
        return;
    }

    // load image data
    pixels = new unsigned char[3 * w * h];

    if (mode == 6)
    {
        f.get();
        f.read( (char*)&(pixels[0]) , 3 * w * h );
    }
    else if (mode == 3)
    {
        for (unsigned int i = 0; i < w * h; i++)
        {
            int v;
            f >> v;
            pixels[3*i + 0] = v;
            f >> v;
            pixels[3*i + 1] = v;
            f >> v;
            pixels[3*i + 2] = v;
        }
    }
    // close file
    f.close();
    if(format == rgb) { return; }
    if(format == rbg) {
        for (unsigned int i = 0; i < w * h; i++)
        {
            unsigned char r = pixels[3*i + 0] , g = pixels[3*i + 1] , b = pixels[3*i + 2];
            pixels[3*i + 0] = r;
            pixels[3*i + 1] = b;
            pixels[3*i + 2] = g;
        }
        return;
    }
}

void load_ppm( std::vector<Vec3>& image, std::string& filename, unsigned int & w, unsigned int & h) {

	unsigned char * pixels;
	load_ppm(pixels,w,h,filename,rgb);
	image = vector< Vec3 >( w*h , Vec3(0,0,0) );
	for (int y=0; y<h; y++){
			for (int x=0; x<w; x++) {
					image[x + y*w][0] = float( pixels[3*(x + y*w) + 0])/RGB_COMPONENT_COLOR;
					image[x + y*w][1] = float( pixels[3*(x + y*w) + 1])/RGB_COMPONENT_COLOR;
					image[x + y*w][2] = float( pixels[3*(x + y*w) + 2])/RGB_COMPONENT_COLOR;
			}
	}
}

}
