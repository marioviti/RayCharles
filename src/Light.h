#ifndef LIGHT_H
#define LIGHT_H
#include "Vec3.h"
#include "Sphere.h"

#include <stdio.h>
#include <time.h>

#define SPHERIC 0
#define MAX_SEED 100000000

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
    int myseed;

  public:

    Light( Sphere _sphere ) {
      init_seed();
      type = SPHERIC;
      ligth_sphere = _sphere;
      position = _sphere.centre;
      color=Vec3(1.,1.,1.);
    }

    Light() {
      init_seed();
      type = SPHERIC;
      ligth_sphere = Sphere(0.01,Vec3(1.,1.,1.));
      position=ligth_sphere.centre;
      color=Vec3(1.,1.,1.);
    }

    void init_seed() {
      srand(time(0));
      myseed = static_cast <int> (rand());
    }

    int grab_seed() {
      myseed = (myseed + 1)%MAX_SEED;
      return myseed;
    }

    Vec3 get_color() {return color;}

    Vec3 get_position() { return position; }
    Vec3 get_sample() {
      float x,y,z;
      while(true) {
        srand(grab_seed());
        x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
        y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
        z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
        if (x*x + y*y + z*z <=1) { return position+ligth_sphere.sphere_ray*(Vec3(x,y,z)); }
     }
      return position;
    }

    RayLightIntersection getIntersection(Ray const & ray ) const {
      RayLightIntersection result;
      result.intersectionExists = false;
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
