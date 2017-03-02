
#ifndef SPHERE_H
#define SPHERE_H
#define PI 3.1415926535
#include <cmath>

class Sphere:public Mesh {
public:
    float ray;
    Vec3 center;
    Sphere():center(0.0,0.0,0.0), ray(1) {}
    Sphere(float _ray, Vec3 _center) { ray = _ray; center = _center; }

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
          p = center + (ray*n);
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
};

#endif
