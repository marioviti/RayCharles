#ifndef TEXTURE_H
#define TEXTURE_H
#include "Vec3.h"

class Texture {
  private:
    int w;
    int h;
  public:
    Vec3 evalue(float u, float v);
    int handleidx;
    int bindidx;
};
#endif
