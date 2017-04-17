#ifndef TEXTURE_H
#define TEXTURE_H

#include "src/Vec3.h"
#include <GL/glew.h> //Always in this order!
#include <GL/gl.h> //glew before gls

class Texture {
  private:
    int w;
    int h;
    GLuint handleidx;
    GLuint bindidx;
    int new_bind_idx();
  public:
    Vec3 evalue(float u, float v);
    void load_texture(std::string& filename);
};
#endif
