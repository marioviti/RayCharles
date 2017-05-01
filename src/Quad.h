#ifndef QUAD_H
#define QUAD_H

#include "Vec3.h"
#include "Vec2.h"
#include <cmath>

struct RayQuadIntersection{
  Material material;
  bool intersectionExists;
  float lambda;
  float u,v;
  unsigned int tIndex;
  Vec3 intersection;
  Vec3 normal;
};

class Quad:public Mesh {
  public:
    float h;
    float w;
    Vec3 x;
    Vec3 y;

    Quad(){};

    void setQuads(Vec3 c1, Vec3 c2, Vec3 c3, Vec3 c4) {
      V.resize(4);
      T.resize(2);
      T[0].v[0] = 0;
      T[0].v[1] = 2;
      T[0].v[2] = 1;

      T[1].v[0] = 1;
      T[1].v[1] = 2;
      T[1].v[2] = 3;

      V[0].p = c1;  V[1].p = c2;  V[2].p = c3;  V[3].p = c4;
    };

    // lb rb lt rt
    void setQuads_with_text(Vec3 lb, Vec3 rb, Vec3 lt, Vec3 rt, Vec2 uvlb, Vec2 uvrb, Vec2 uvlt, Vec2 uvrt) {
      V.resize(4);
      T.resize(2);
      UVs.resize(4);
      T[0].v[0] = 0;
      T[0].v[1] = 2;
      T[0].v[2] = 1;

      T[1].v[0] = 1;
      T[1].v[1] = 2;
      T[1].v[2] = 3;

      V[0].p = lb;  V[1].p = rb;  V[2].p = lt;  V[3].p = rt;
      UVs[0] = uvlb;   UVs[1] = uvrb;   UVs[2] = uvlt;   UVs[3] = uvrt;
      x = rb - lb;  w = x.length(); x.normalize();
      y = lt - lb;  h = y.length(); y.normalize();
    };

    Vec2 calculate_uv(Vec3 const p) const {
      float u;
      float v;
      u = (Vec3::dot(p-V[0].p ,x)/w);
      v = (Vec3::dot(p-V[0].p ,y)/h);
      return Vec2(u,v);
    };

    RayQuadIntersection getIntersection(Ray const & ray) {
        RayQuadIntersection result;
        result.intersectionExists = false;
        result.lambda = MESH_MAX_LAMBDA;
        RayTriangleIntersection rays_Triangle_intersection;
        for(unsigned int i = 0; i < T.size(); ++i) {
          Triangle tri (V[ T[i].v[0] ].p, V[ T[i].v[1] ].p, V[ T[i].v[2] ].p);
          rays_Triangle_intersection = tri.getIntersection(ray);
          if(rays_Triangle_intersection.intersectionExists) {
            if( rays_Triangle_intersection.lambda > MESH_MIN_LAMBDA_EPSILON && rays_Triangle_intersection.lambda < result.lambda){
              result.intersectionExists = rays_Triangle_intersection.intersectionExists;
              result.lambda = rays_Triangle_intersection.lambda;
              result.intersection = rays_Triangle_intersection.intersection;
              result.normal = rays_Triangle_intersection.normal;
              if (has_texture == 1) {
                Vec2 uv = calculate_uv(result.intersection);
                result.u = uv[0];
                result.v = uv[1];
              }
            }
          }
        }
        result.material = this->material;
        return result;
    };
};


#endif
