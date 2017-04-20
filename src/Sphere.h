#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>

#define PI 3.1415926535
#include "Mesh.h"
#include "Material.h"
#include "Ray.h"
#include "Vec2.h"

struct RaySphereIntersection{
  Material material;
  bool intersectionExists;
  float lambda;
  float u,v;
  unsigned int tIndex;
  Vec3 intersection;
  Vec3 normal;
};

class Sphere:public Mesh {
public:
    float sphere_ray;
    Vec3 centre;
    int paralles;
    int meridians;
    Sphere():centre(0.0,0.0,0.0), sphere_ray(1) {}
    Sphere(float _sphere_ray, Vec3 _centre) { sphere_ray = _sphere_ray; centre = _centre; }

    void buildMesh(int N, int M);
    float solid_angle(Vec3 const & p);
    RaySphereIntersection getIntersection( Ray const & ray ) const;
    Vec2 calculate_uv(Vec3 const n) const;
};

#endif
