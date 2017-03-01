#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "Mesh.h"

#include <GL/glut.h>

class Scene {
    // Mettez ici tout ce que vous souhaitez avoir dans votre scene 3D.
    // Pour l'instant, on a uniquement des maillages, mais par la suite on pourra rajouter des objets specialises comme des spheres, des cylindres ou des cones par ex...
    //std::vector< Mesh > meshes;

public:
    std::vector< Mesh > meshes;
    Scene() {}

    void addMesh(std::string const & modelFilename) {
        meshes.resize( meshes.size() + 1 );
        Mesh & meshAjoute = meshes[ meshes.size() - 1 ];
        meshAjoute.loadOFF (modelFilename);
        meshAjoute.centerAndScaleToUnit ();
        meshAjoute.recomputeNormals ();
        meshAjoute.buildArray ();
    }

    void draw() const {
        // iterer sur l'ensemble des objets, et faire leur rendu.
        for( unsigned int mIt = 0 ; mIt < meshes.size() ; ++mIt ) {
            Mesh const & mesh = meshes[mIt];
            mesh.draw();
            mesh.drawCage();

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
