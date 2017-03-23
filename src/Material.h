#ifndef MATERIAL_H
#define MATERIAL_H

#define DIFFUSE 0
#define SPECULAR 1
#include "Vec3.h"

class Material {
  private:
    int type;
    Vec3 color;
  public:
    Material() { type=DIFFUSE };
    void set_color(Vec3 _color) { color = _color; }
    void set_type(int _type) { type = _type; }
    int get_type() { return type; }
    Vec3 get_color() { return color; }
};

#endif
