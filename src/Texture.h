#ifndef TEXTURE_H
#define TEXTURE_H

#include "Vec3.h"
#include "src/PPMIO.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vec3.h"

class Texture {
  private:
    int w;
    int h;
  public:
    Vec3 evalue(float u, float v);
    void load_texture();
    int handleidx;
    int bindidx;
};
#endif
