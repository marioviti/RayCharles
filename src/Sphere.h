
#ifndef SPHERE_H
#define SPHERE_H
#define PI 3.1415926535
#include <cmath>
#include "Material.h"

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
    Sphere():centre(0.0,0.0,0.0), sphere_ray(1) {}
    Sphere(float _sphere_ray, Vec3 _centre) { sphere_ray = _sphere_ray; centre = _centre; }

    void buildMesh(int N, int M) {
      V.resize(N*M);
      float u,v,theta,phi,x,y,z;
      Vec3 p;
      Vec3 n;
      for (unsigned int i=0; i<N; ++i) {
        for (unsigned int j=0; j<M; ++j) {
          u = float(i)/(N-1);
          v = float(j)/(M-1);
          theta = u*2*PI;
          phi = -PI/2 + v*PI;
          x = std::cos(phi)*std::cos(theta);
          y = std::cos(phi)*std::sin(theta);
          z = std::sin(phi);
          n = Vec3 (x,y,z);
          p = centre + (sphere_ray*n);
          V[i+j*N].p = p;
          V[i+j*N].n = n;
        }
      }
      unsigned int ind0,ind1,ind2,ind3;
      for (unsigned int i=0; i<N-1; ++i) {
        for (unsigned int j=0; j<M-1; ++j) {
          ind0 = i+j*N;
          ind1 = (i+1)+j*N;
          ind2 = (i+1)+(j+1)*N;
          ind3 = i+(j+1)*N;
          T.push_back(MeshTriangle(ind0,ind1,ind2));
          T.push_back(MeshTriangle(ind0,ind2,ind3));
        }
      }
      buildArray();
    }

    RaySphereIntersection getIntersection( Ray const & ray ) const {
      RaySphereIntersection res;
      res.intersectionExists=false;
      float delta;
      delta = pow(2*Vec3::dot((ray.origin()-centre),ray.direction()),2)-4*(pow((ray.origin()-centre).length(),2)-pow(sphere_ray,2));

      if (delta > 0) {
           float eps_Min = (-2*Vec3::dot((ray.origin()-centre),ray.direction())-sqrt(delta))/2;
           float eps_Max = (-2*Vec3::dot((ray.origin()-centre),ray.direction())+sqrt(delta))/2;
           if (eps_Min >= 0) {
                res.intersectionExists = true,
                res.lambda =eps_Min;
                res.intersection = ray.origin() + eps_Min*ray.direction();
                res.normal = (centre-res.intersection);
                res.normal.normalize();
           }
           else if (eps_Max >=0){
                res.intersectionExists = true,
                res.lambda =eps_Max;
                res.intersection = ray.origin() + eps_Max*ray.direction();
                res.normal = (centre-res.intersection);
                res.normal.normalize();
           }
      }
      res.material = this->material;
      return res;
   }
};

#endif
