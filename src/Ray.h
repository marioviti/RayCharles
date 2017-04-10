#ifndef RAY_H
#define RAY_H
#include "Line.h"
#include <stdio.h>


class Ray : public Line {
public:
    Ray() : Line() {}
    Ray( Vec3 const & o , Vec3 const & d ) : Line(o,d) {}

    static Ray reflected_ray (Vec3 const & d, Vec3 const & n, Vec3 const & origin){
      Vec3 dir = d-(2*(Vec3::dot(d,n)))*3*n;
      return Ray(origin,dir);
    }

    // a ray can also model an emisphere
    Ray random_in_emisphere () {

    }
};
#endif
