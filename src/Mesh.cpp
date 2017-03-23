#include "Mesh.h"
#include <iostream>
#include <fstream>

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
