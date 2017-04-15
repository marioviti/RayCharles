#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vec3.h"
#define RGB_COMPONENT_COLOR 255

namespace PPMIO {
using namespace std;

void eat_comment(ifstream &f);

struct RGB {
    unsigned char r, g, b;
};

struct ImageRGB {
    int w, h;
    vector<RGB> data;
};

enum loadedFormat { rgb, rbg };
void load_ppm( unsigned char * & pixels , unsigned int & w , unsigned int & h , const string &name , loadedFormat format = rgb);
void load_ppm( std::vector<Vec3>& image, std::string& filename, unsigned int & w, unsigned int & h);
void write_ppm( std::vector<Vec3>& image, std::string& filename, int w, int h);
}

#endif
