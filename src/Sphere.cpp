#include "Sphere.h"
#include "Mesh.h"
#include "Material.h"
#include "Ray.h"

/*
void calculate_uv(Vec3 n) {

  //phi,theta -> u,v = [0-2Pi,0-2Pi] -> [0-1,0-1]

  float phi = std::acos(n[1]);
  float theta = std::asin(n[0]/std::sin(phi));
  float u = phi/2*PI;
  float v = theta/2*PI;
}
*/

float Sphere::solid_angle(Vec3 const & p) {
  Vec3 distance_v = p-centre;
  float square_dist = Vec3::dot(distance_v,distance_v);
  float square_ratio = sphere_ray/square_dist;
  return 4*PI*square_ratio;
}

void Sphere::buildMesh(int N, int M) {
  if(N%2==0) N++;
  if(M%2==0) M++;
  V.resize(N*M);
  float u,v,theta,phi,x,y,z;
  Vec3 p;
  Vec3 n;
  for (unsigned int i=0; i<N; ++i) {
    for (unsigned int j=0; j<M; ++j) {
      u = float(i)/(N-1);
      v = float(j)/(M-1);
      /*
      theta = u*2*PI;
      phi = -PI/2 + v*PI;
      x = std::cos(phi)*std::cos(theta);
      y = std::cos(phi)*std::sin(theta);
      z = std::sin(phi);
      */
      theta = u*2*PI;
      phi = v*2*PI;
      x = std::sin(phi)*std::sin(theta);
      y = std::cos(phi);
      z = std::sin(phi)*std::cos(theta);
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
      T.push_back(MeshTriangle(ind0,ind2,ind3));
      T.push_back(MeshTriangle(ind0,ind1,ind2));

    }
  }
  buildArray();
}

RaySphereIntersection Sphere::getIntersection( Ray const & ray ) const {
  RaySphereIntersection res;
  res.intersectionExists = false;
  float delta;
  delta = pow(2*Vec3::dot((ray.origin()-centre),
    ray.direction()),2)-4*(pow((ray.origin()-centre).length(),2)-pow(sphere_ray,2));
  if (delta > 0) {
    float eps_Min = (-2*Vec3::dot((ray.origin()-centre),ray.direction())-sqrt(delta))/2;
    float eps_Max = (-2*Vec3::dot((ray.origin()-centre),ray.direction())+sqrt(delta))/2;
    if (eps_Min >= 0) {
      res.intersectionExists = true,
      res.lambda =eps_Min;
      res.intersection = ray.origin() + eps_Min*ray.direction();
      res.normal = (res.intersection-centre);
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