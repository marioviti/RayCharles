#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "Instance.h"
#include "Vec3.h"
#include "Vec2.h"
#include "Ray.h"
#include "Triangle.h"
#include "Material.h"
#include "src/GLProgram.h"
#include <GL/glut.h>

#define MESH_MIN_LAMBDA_EPSILON 0.0000001
#define MESH_MAX_LAMBDA 1000

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
        coordU = v.coordU;
        coordV = v.coordV;
        return (*this);
    }
    // membres :
    float coordU, coordV;
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

class Mesh:public Instance {

public:

    int bindindex;
    int has_texture; //  this is for opengl
    // Material
    Material material;
    Mesh() { material=Material::default_material(); has_texture=0; }
    void set_material(const Material & my_material) { material=my_material; }
    Material get_material() { return material; }

    std::vector<MeshVertex> V;
    std::vector<MeshTriangle> T;

    // initialize vetctors to bind with graphic card buffers.
    std::vector<unsigned int> triangles;
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<Vec2> UVs;

    // vectors holding the cage settings
    std::vector<unsigned int> cageOrder;
    std::vector<Vec3> cageVertices;

    void set_texture_index(int bindindex_) { material.set_texture_index(bindindex_); bindindex = bindindex_; has_texture=1; }
    int get_texture_index() { return bindindex; }
    int unset_texture_index() { has_texture = 0; }

    // Rendering routines
    RayMeshIntersection getIntersection(Ray const & ray);

    // Create routines
    void setQuads(Vec3 c1, Vec3 c2, Vec3 c3, Vec3 c4);
    void loadOFF (const std::string & filename);
    void recomputeNormals ();
    void centerAndScaleToUnit ();
    void scaleUnit ();
    void buildArray();
    void build_cage();
    void draw() const;
    void drawWithTexture() const;
    void drawCage() const;
};

#endif
