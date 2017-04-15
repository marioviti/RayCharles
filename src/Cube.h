#ifndef SCUBE_H
#define SCUBE_H

#include <cmath>

#define PI 3.1415926535
#include "Mesh.h"
#include "Material.h"
#include "Ray.h"

struct RayCubeIntersection{
  Material material;
  bool intersectionExists;
  float lambda;
  float u,v;
  unsigned int tIndex;
  Vec3 intersection;
  Vec3 normal;
};

class Cube:public Mesh {
public:
    float side;
    Vec3 centre;
    Cube():centre(0.0,0.0,0.0), side(1) {}
    Cube(float _side, Vec3 _centre) { side = _side; centre = _centre; }
    void buildMesh(int N);
};

#endif
