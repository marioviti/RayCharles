#ifndef LIGHT_H
#define LIGHT_H
#include "Vec3.h"
#include "Sphere.h"
#include "Instance.h"

#include <stdio.h>
#include <time.h>

#define SPHERIC 0
#define LIGHT_MAX_SEED 100000000

struct RayLightIntersection{
  bool intersectionExists;
  float lambda;
  float u,v;
  unsigned int tIndex;
  Vec3 intersection;
  Vec3 normal;
  Vec3 color;
};

class Light:public Instance {
  private:
    int type;
    Sphere ligth_sphere;
    Vec3 position;
    Vec3 color;
    int myseed;

  public:
    Light() { init_seed(); }

    Light(Vec3 const & position_) : Light() {
      type = SPHERIC;
      ligth_sphere = Sphere(0.1,position_);
      position=ligth_sphere.centre;
      color=Vec3(1.,0.99,0.99);
    }

    void init_seed() {
      srand(time(0));
      myseed = static_cast <int> (rand());
    }

    float solid_angle(Vec3 const & p) {
      if (type == SPHERIC)
        return ligth_sphere.solid_angle(p);
      return 1.f;
    }

    int grab_seed() {
      myseed = (myseed + 1)%LIGHT_MAX_SEED;
      return myseed;
    }

    Vec3 get_color() { return color; }
    Vec3 get_position() { return position; }
    Vec3 get_sample() {
      float x,y,z;
      srand(grab_seed());
      x = (static_cast <float> ( (rand()) / (static_cast <float> (RAND_MAX)) ))*2 -1;
      y = (static_cast <float> ( (rand()) / (static_cast <float> (RAND_MAX)) ))*2 -1;
      z = (static_cast <float> ( (rand()) / (static_cast <float> (RAND_MAX)) ))*2 -1;
      return position+ligth_sphere.sphere_ray*(1-10e-2)*(Vec3(x,y,z).normalize());
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
