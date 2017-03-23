#ifndef LIGHT_H
#define LIGHT_H
#include "Vec3.h"

class Light {
  private:
    Vec3 position;
  public:
    Light( Vec3 _position) { position = _position; }
    Vec3 get_position() { return position; }
};

#endif
