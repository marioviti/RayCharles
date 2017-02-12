# RayCharles
RayTracer with realtime Editor: Combining realtime 3D acceleration with photorealistic rendering.

## OpenGL Basics.

Modern OpenGL tends to minimize calls between Cpu and Gpu as each
call means memory transfert which can lead to serious performance
reduction. SIMD is the way of thinking, all operations should be
designed to work on batches of data.

An example of old OpenGL:

    // loading vertices into memory for render.
    glBegin (GL_TRIANGLES);
      for (unsigned int i = 0; i < T.size (); i++)
           for (unsigned int j = 0; j < 3; j++) {
               const MeshVertex & v = V[T[i].v[j]];
               glNormal3f (v.n[0], v.n[1], v.n[2]);
               glVertex3f (v.p[0], v.p[1], v.p[2]);
      }
    glEnd ();

every vertex and normal is loaded with an instruction.
this generates traffic as each called is blocked untill
GPU returns.

OpenGL provides calls to bind std vectors to GPU buffers.
so before rendering load the array.

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

This approach has many plus but also a minus, you'll have to use
another array to store the order of occurrence of each vertex in order to render triangels. so if the first 2 triangles have 4 vertices stored as v1 v2 v3 v4 the order should be 0 1 2, 1 2 3
to tell to assign to 2 triangles the vertices, and normals to the
right triangles.

To render you'll just have to call the bindings functions once

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 3*sizeof(float),(GLvoid*)(&vertices[0]));
    glNormalPointer( GL_FLOAT, 3*sizeof(float),(GLvoid*)(&normals[0]));
    glDrawElements(GL_TRIANGLES,triangles.size(),GL_UNSIGNED_INT,(GLvoid*)(&triangles[0]));		
