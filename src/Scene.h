#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "Sphere.h"
#include "Ray.h"

#include <GL/glut.h>

class Scene {
    // Mettez ici tout ce que vous souhaitez avoir dans votre scene 3D.
    // Pour l'instant, on a uniquement des maillages, mais par la suite on pourra rajouter des objets specialises comme des spheres, des cylindres ou des cones par ex...

private:
    std::vector< Mesh > meshes;
    std::vector< Sphere > spheres;
    GLProgram* gl_Program;

public:
    Scene() {}

    float rayTrace(Ray rayon, std::vector<Vec3> rays) {
      for( unsigned int mIt = 0 ; mIt < meshes.size() ; ++mIt ) {

      }
    }

    void addSphere(float _ray, Vec3 _center) {
      Sphere sphere = Sphere(_ray,_center);
      sphere.buildMesh(10,10);
      spheres.push_back(sphere);
    }

    void addMesh(std::string const & modelFilename) {
        meshes.resize( meshes.size() + 1 );
        Mesh & meshAjoute = meshes[ meshes.size() - 1 ];
        meshAjoute.loadOFF (modelFilename);
        meshAjoute.centerAndScaleToUnit ();
        meshAjoute.recomputeNormals ();
        meshAjoute.buildArray();
    }

    void addGLProgram(GLProgram* _gl_Program) {
      gl_Program = _gl_Program;
    }

    void draw() const {
        // iterer sur l'ensemble des objets, et faire leur rendu.
        for( unsigned int mIt = 0 ; mIt < meshes.size() ; ++mIt ) {
            Mesh const & mesh = meshes[mIt];
            gl_Program->use();
            mesh.draw();
            gl_Program->stop();
            //mesh.drawCage();

            /*
            // copies affichees : (Exercice 3)
            glTranslatef( .  ,  .  ,  .  ); glRotatef( .  ,  .  ,  .  ,  .  ); glScalef( .  ,  .  ,  .  );
            mesh.draw();
            glTranslatef( .  ,  .  ,  .  ); glRotatef( .  ,  .  ,  .  ,  .  ); glScalef( .  ,  .  ,  .  );
            mesh.draw();
            */
        }
        for( unsigned int mIt = 0 ; mIt < spheres.size() ; ++mIt ) {
            Sphere const & sphere = spheres[mIt];
            sphere.draw();
            //sphere.drawCage();
            /*
            // copies affichees : (Exercice 3)
            glTranslatef( .  ,  .  ,  .  ); glRotatef( .  ,  .  ,  .  ,  .  ); glScalef( .  ,  .  ,  .  );
            mesh.draw();
            glTranslatef( .  ,  .  ,  .  ); glRotatef( .  ,  .  ,  .  ,  .  ); glScalef( .  ,  .  ,  .  );
            mesh.draw();
            */
        }
    }
};



#endif
