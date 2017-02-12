#ifndef MESH_H
#define MESH_H


#include <vector>
#include <string>
#include "Vec3.h"

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




class Mesh {
public:
    std::vector<MeshVertex> V;
    std::vector<MeshTriangle> T;

    // initialize vetctors to bind with graphic card buffers.
    std::vector<unsigned int> triangles;
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;

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
	  }

    void draw() const {
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);

      glVertexPointer(3, GL_FLOAT, 3*sizeof(float),(GLvoid*)(&vertices[0]));
      glNormalPointer(GL_FLOAT, 3*sizeof(float),(GLvoid*)(&normals[0]));
      glDrawElements(GL_TRIANGLES,triangles.size(),GL_UNSIGNED_INT,(GLvoid*)(&triangles[0]));
    }
};



#endif
