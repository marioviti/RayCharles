#ifndef LIGHT_H
#define LIGHT_H
#include "Vec3.h"
#include "Sphere.h"

#include <stdio.h>
#include <time.h>

#define SPHERIC 0

struct RayLightIntersection{
  bool intersectionExists;
  float lambda;
  float u,v;
  unsigned int tIndex;
  Vec3 intersection;
  Vec3 normal;
  Vec3 color;
};

class Light {

  private:
    int type;
    Sphere ligth_sphere;
    Vec3 position;
    Vec3 color;

  public:
    Light( Sphere _sphere ) { type = SPHERIC; ligth_sphere = _sphere; position = _sphere.centre; }
    Light( Vec3 _position ) { position = _position; }
    Light() { type = SPHERIC; ligth_sphere = Sphere(0.01,Vec3(1.,1.,1.)); position=ligth_sphere.centre; color=Vec3(1.,1.,1.); }

    Vec3 get_color() {return color;}

    Vec3 get_position() { return position; }
    Vec3 get_sample() {
      srand(time(0));
      float x,y,z;
      while(true) {
        x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
        y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
        z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
        if (x*x + y*y + z*z <=1) { return position+ligth_sphere.sphere_ray*(Vec3(x,y,z)); }
     }
      return position;
    }

    RayLightIntersection getIntersection(Ray const & ray ) const {
      RayLightIntersection result;
      if(type==SPHERIC) {
        RaySphereIntersection intersection;
        intersection = ligth_sphere.getIntersection(ray);
        result.intersectionExists = intersection.intersectionExists;
        result.lambda = intersection.lambda;
        result.intersection = intersection.intersection;
        result.normal = intersection.normal;
        result.color = color;
      }
      return result;
    }
};

#endif
