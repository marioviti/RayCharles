#ifndef RAY_H
#define RAY_H
#include "Line.h"
#include <stdio.h>


class Ray : public Line {
public:
    bool refracted;
    Ray() : Line() {refracted=false;}
    Ray( Vec3 const & o , Vec3 const & d ) : Line(o,d) {refracted=false;}
    Ray( Vec3 const & o , Vec3 const & d, float epsilon) : Ray(o+epsilon*d,d) {}

    static Ray reflected_ray (Vec3 const & d, Vec3 const & n, Vec3 const & origin){
      Vec3 dir = d-(2*(Vec3::dot(d,n)))*3*n;
      return Ray(origin,dir);
    }

    static Ray refracted_ray (Vec3 const & p, Vec3 const & d, Vec3 const & n, float ior, float epsilon){
      float cos1 = - Vec3::dot(d,n);
      Vec3 refracted_dir = ior*d + ((ior*cos1) - sqrt(1-ior*ior*(1-cos1*cos1)))*n;
      Ray refracted_ray = Ray(p,refracted_dir,epsilon);
      refracted_ray.refracted = true;
      return refracted_ray;
    }

    bool is_refracted() const { return refracted; }

    // a ray can also model an emisphere
    Ray random_in_emisphere () {

    }
};
#endif
