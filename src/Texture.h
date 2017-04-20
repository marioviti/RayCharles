#ifndef TEXTURE_H
#define TEXTURE_H

#include "src/Vec3.h"
#include <vector>
//#include <iostream>
#include <GL/glew.h> //Always in this order!
#include <GL/gl.h> //glew before gls

class Texture {
  private:
    int w;
    int h;
    GLuint handleIndex;
    int bindIndex;
    int new_bind_idx();
    unsigned char *imageTexture;
    std::vector<Vec3> vecImageTexture;
  public:
    Texture(){};
    int static bind_index_gen;
    Vec3 evalue(float u, float v); // will return the pixel value from u,v coords
    void load_texture(std::string& filename);
    int get_bindIndex() {return bindIndex;};
};
#endif
