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

    static Ray reflected_ray (Vec3 const & origin, Vec3 const & direction, Vec3 const & n){
      //Rr = Ri - 2 N (Ri . N)
      Vec3 refl_dir = ( direction - 2.0*n*Vec3::dot(direction, n));
      return Ray(origin,refl_dir);
    }

    static Ray refracted_ray (Vec3 const & p, Vec3 & i, Vec3 const & n, float ior){
      float n1,n2,n_ior,cos_theta_t,sin_theta_t_sq;
      float cos_theta_i = Vec3::dot(i,n);
      i.normalize();
      Vec3 normal = n;
      if(cos_theta_i>0.0) {
        n1 = ior;
        n2 = 1.0;
        normal = -1*normal;
      }
      else {
        n1 = 1.0;
        n2 = ior;
        cos_theta_i = -cos_theta_i;
      }
      n_ior = n1/n2;
      if (n_ior==1.0) {
        // no refraction
        Ray refracted_ray = Ray(p,i);
        return refracted_ray;
      }
      sin_theta_t_sq = n_ior*n_ior*(1.0-(cos_theta_i*cos_theta_i));
      if (1-sin_theta_t_sq<0.0) {
        // total internal refraction
        return reflected_ray(p,i,normal);
      }
      cos_theta_t = sqrt(1-sin_theta_t_sq);
      Vec3 trasmission = (n_ior * i) + ((n_ior*cos_theta_i - cos_theta_t) * normal);
      Ray refracted_ray = Ray(p,trasmission);
      return refracted_ray;
    }

    bool is_refracted() const { return refracted; }

};
#endif
