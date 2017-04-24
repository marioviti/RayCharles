#include "Mesh.h"
#include <iostream>
#include <fstream>

RayMeshIntersection Mesh::getIntersection(Ray const & ray) {
    RayMeshIntersection result;
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

void Mesh::buildArray() {
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

void Mesh::build_cage() {

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

void Mesh::drawCage() const {
  glDisable(GL_LIGHTING);
  glColor3f(0.3,0.3,1);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 3*sizeof(float),(GLvoid*)(&cageVertices[0]));
  glDrawElements(GL_LINES,cageOrder.size(),GL_UNSIGNED_INT,(GLvoid*)(&cageOrder[0]));
  glEnable(GL_LIGHTING);
}


void Mesh::draw() const {
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 3*sizeof(float),(GLvoid*)(&vertices[0]));
  glNormalPointer(GL_FLOAT, 3*sizeof(float),(GLvoid*)(&normals[0]));
  glDrawElements(GL_TRIANGLES,triangles.size(),GL_UNSIGNED_INT,(GLvoid*)(&triangles[0]));
}

void Mesh::drawWithTexture() const {
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState (GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(3, GL_FLOAT, 3*sizeof(float),(GLvoid*)(&vertices[0]));
  glNormalPointer(GL_FLOAT, 3*sizeof(float),(GLvoid*)(&normals[0]));
  glTexCoordPointer (2, GL_FLOAT, 2*sizeof (float), (GLvoid*)(&UVs[0]));
  glDrawElements(GL_TRIANGLES,triangles.size(),GL_UNSIGNED_INT,(GLvoid*)(&triangles[0]));
}

/*
void Mesh::draw() const {
    // This code is deprecated.
    glBegin (GL_TRIANGLES);
    for (unsigned int i = 0; i < T.size (); i++)
      for (unsigned int j = 0; j < 3; j++) {
        const MeshVertex & v = V[T[i].v[j]];
        glNormal3f (v.n[0], v.n[1], v.n[2]);
        glVertex3f (v.p[0], v.p[1], v.p[2]);
      }
    glEnd ();
}

void Mesh::drawWithTexture() const {
    // This code is deprecated.
    glBegin (GL_TRIANGLES);
    for (unsigned int i = 0; i < T.size (); i++)
      for (unsigned int j = 0; j < 3; j++) {
        const MeshVertex & vertex = V[T[i].v[j]];
        glTexCoord2f(UVs[T[i].v[j]][0] , UVs[T[i].v[j]][1] );
        glNormal3f (vertex.n[0], vertex.n[1], vertex.n[2]);
        glVertex3f (vertex.p[0], vertex.p[1], vertex.p[2]);
      }
    glEnd ();
}
*/

void Mesh::loadOFF (const std::string & filename) {
    std::ifstream in (filename.c_str ());
    if (!in)
        exit (EXIT_FAILURE);
    std::string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    V.resize (sizeV);
    T.resize (sizeT);
    for (unsigned int i = 0; i < sizeV; i++)
        in >> V[i].p;
    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++)
            in >> T[i].v[j];
    }
    in.close ();
}

void Mesh::setQuads(Vec3 c1, Vec3 c2, Vec3 c3, Vec3 c4) {
  V.resize(4);
  T.resize(2);
  T[0].v[0] = 0;
  T[0].v[1] = 2;
  T[0].v[2] = 1;

  T[1].v[0] = 1;
  T[1].v[1] = 2;
  T[1].v[2] = 3;

  V[0].p = c1;  V[1].p = c2;  V[2].p = c3;  V[3].p = c4;
}

void Mesh::recomputeNormals () {
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n = Vec3 (0.0, 0.0, 0.0);
    for (unsigned int i = 0; i < T.size (); i++) {
        Vec3 e01 = V[T[i].v[1]].p -  V[T[i].v[0]].p;
        Vec3 e02 = V[T[i].v[2]].p -  V[T[i].v[0]].p;
        Vec3 n = Vec3::cross (e01, e02);
        n.normalize ();
        for (unsigned int j = 0; j < 3; j++)
            V[T[i].v[j]].n += n;
    }
    for (unsigned int i = 0; i < V.size (); i++)
        V[i].n.normalize ();
}

void Mesh::centerAndScaleToUnit () {
    Vec3 c(0,0,0);
    for  (unsigned int i = 0; i < V.size (); i++)
        c += V[i].p;
    c /= V.size ();
    float maxD = (V[0].p - c).length();
    for (unsigned int i = 0; i < V.size (); i++){
        float m = (V[i].p - c).length();
        if (m > maxD)
            maxD = m;
    }
    for  (unsigned int i = 0; i < V.size (); i++)
        V[i].p = (V[i].p - c) / maxD;
}
