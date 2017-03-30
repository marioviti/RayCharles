#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "Vec3.h"
#include "Ray.h"
#include "Triangle.h"
#include "Material.h"
#include "src/GLProgram.h"
#include <GL/glut.h>

// -------------------------------------------
// Basic Mesh class
// -------------------------------------------

struct MeshVertex {
    inline MeshVertex () {}
    inline MeshVertex (const Vec3 & _p, const Vec3 & _n) : p (_p), n (_n) {}
    inline MeshVertex (const MeshVertex & v) : p (v.p), n (v.n) {}
    inline virtual ~MeshVertex () {}
    inline MeshVertex & operator = (const MeshVertex & v) {
        p = v.p;
        n = v.n;
        return (*this);
    }
    // membres :
    Vec3 p; // une position
    Vec3 n; // une normale
};

struct MeshTriangle {
    inline MeshTriangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline MeshTriangle (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline MeshTriangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    inline virtual ~MeshTriangle () {}
    inline MeshTriangle & operator = (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :

    unsigned int v[3];
};


struct RayMeshIntersection{
  Material material;
  bool intersectionExists;
  float lambda;
  float u,v;
  unsigned int tIndex;
  Vec3 intersection;
  Vec3 normal;
};

class Mesh {

public:

    // Material
    Material material;
    Mesh() { material=Material::default_material(); }
    void set_material(const Material & my_material) { material=my_material; }
    Material get_material() { return material; }

    std::vector<MeshVertex> V;
    std::vector<MeshTriangle> T;

    // initialize vetctors to bind with graphic card buffers.
    std::vector<unsigned int> triangles;
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;

    // vectors holding the cage settings
    std::vector<unsigned int> cageOrder;
    std::vector<Vec3> cageVertices;

    void setQuads(Vec3 c1, Vec3 c2, Vec3 c3, Vec3 c4);
    void loadOFF (const std::string & filename);
    void recomputeNormals ();
    void centerAndScaleToUnit ();
    void scaleUnit ();

    void buildArray() {
  		for (unsigned int i = 0; i < T.size(); i++) {
  			triangles.push_back(T[i].v[0]);
  			triangles.push_back(T[i].v[1]);
  			triangles.push_back(T[i].v[2]);
  		}
  		for (unsigned int i = 0; i < V.size(); i++) {
  			vertices.push_back(V[i].p);
  			normals.push_back(V[i].n);
  		}
      build_cage();
	  }

    RayMeshIntersection getIntersection(Ray const & ray) {
        RayMeshIntersection result;
        result.intersectionExists = false;
        RayTriangleIntersection rays_Triangle_intersection;
        for(unsigned int i = 0; i < T.size(); ++i) {
          Triangle tri (V[ T[i].v[0] ].p, V[ T[i].v[1] ].p, V[ T[i].v[2] ].p);
          rays_Triangle_intersection = tri.getIntersection(ray);
          if(rays_Triangle_intersection.intersectionExists) {
            if(result.lambda < rays_Triangle_intersection.lambda){
              result.intersectionExists = rays_Triangle_intersection.intersectionExists;
              result.lambda = rays_Triangle_intersection.lambda;
              result.u = rays_Triangle_intersection.u;
              result.v = rays_Triangle_intersection.v;
              result.intersection = rays_Triangle_intersection.intersection;
              result.normal = rays_Triangle_intersection.normal;
            }
          }
        }
        result.material = this->material;
        return result;
    }

    void build_cage() {

      float max_x = 0.0, min_x = 0.0,
            max_y =0.0, min_y = 0.0,
            max_z =0.0, min_z = 0.0;

      for (unsigned int i = 0; i < V.size(); i++) {
        if (V[i].p[0] > max_x) max_x = V[i].p[0];
        if (V[i].p[0] < min_x) min_x = V[i].p[0];
        if (V[i].p[1] > max_y) max_y = V[i].p[1];
        if (V[i].p[1] < min_y) min_y = V[i].p[1];
        if (V[i].p[2] > max_z) max_z = V[i].p[2];
        if (V[i].p[2] < min_z) min_z = V[i].p[2];
      }

      cageVertices.push_back(Vec3( max_x , max_y , max_z ));
      cageVertices.push_back(Vec3( max_x , min_y , max_z ));
      cageVertices.push_back(Vec3( min_x , min_y , max_z ));
      cageVertices.push_back(Vec3( min_x , max_y , max_z ));
      cageVertices.push_back(Vec3( min_x , max_y , min_z ));
      cageVertices.push_back(Vec3( max_x , max_y , min_z ));
      cageVertices.push_back(Vec3( max_x , min_y , min_z ));
      cageVertices.push_back(Vec3( min_x , min_y , min_z ));

      for (unsigned int i = 1; i < cageVertices.size(); i++) {
        cageOrder.push_back(i-1);
        cageOrder.push_back(i);
      }
      cageOrder.push_back(7);cageOrder.push_back(4);
      cageOrder.push_back(7);cageOrder.push_back(2);
      cageOrder.push_back(5);cageOrder.push_back(0);
      cageOrder.push_back(6);cageOrder.push_back(1);
      cageOrder.push_back(3);cageOrder.push_back(0);
    }

    void draw() const {
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);

      glVertexPointer(3, GL_FLOAT, 3*sizeof(float),(GLvoid*)(&vertices[0]));
      glNormalPointer(GL_FLOAT, 3*sizeof(float),(GLvoid*)(&normals[0]));
      glDrawElements(GL_TRIANGLES,triangles.size(),GL_UNSIGNED_INT,(GLvoid*)(&triangles[0]));
    }

    void drawCage() const {

      glDisable(GL_LIGHTING);
      glColor3f(0.3,0.3,1);
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 3*sizeof(float),(GLvoid*)(&cageVertices[0]));
      glDrawElements(GL_LINES,cageOrder.size(),GL_UNSIGNED_INT,(GLvoid*)(&cageOrder[0]));
      glEnable(GL_LIGHTING);
    }
};



#endif
