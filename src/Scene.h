#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "Sphere.h"
#include "Ray.h"

#include <GL/glut.h>

struct RaySceneIntersection {
  bool intersectionExists;
  float lambda;
  Vec3 intersection;
  Vec3 normal;
};

class Scene {
    // Mettez ici tout ce que vous souhaitez avoir dans votre scene 3D.
    // Pour l'instant, on a uniquement des maillages, mais par la suite on pourra rajouter des objets specialises comme des spheres, des cylindres ou des cones par ex...

private:
    std::vector< Mesh > meshes;
    std::vector< Sphere > spheres;
    GLProgram* gl_Program;

public:
    Scene() {}

    Vec3 rayTrace(Ray const & ray, std::vector<Vec3> rays_intersections, int depth) {
      Vec3 color_value;
      RaySceneIntersection result;
      RayMeshIntersection rayMeshIntersection;
      for(unsigned int mIt = 0 ; mIt < meshes.size() ; ++mIt ) {
        rayMeshIntersection = meshes[mIt].getIntersection(ray);
        if(rayMeshIntersection.intersectionExists && rayMeshIntersection.lambda<result.lambda) {
          result.intersectionExists = true;
          result.lambda = rayMeshIntersection.lambda;
          result.intersection = rayMeshIntersection.intersection;
          result.normal = rayMeshIntersection.normal;
        }
      }
      // we could handle rebounds at another level, like a mesh level
      // but suppose we do that, we may calculate all the rebounds for nothing.
      // so we'll need to get information from the object material.

      // push back the latest ray intersection
      if(result.intersectionExists) {
        rays_intersections.push_back(result.intersection);
        // do not reflect
        if (depth == 0) { return color_value; }
        // do reflect
        if (result.intersectionExists) {
          // calculate reflected ray
          Vec3 reflected_direction = -1*ray.direction() + (2*Vec3::dot(ray.direction(),(result.normal)))*result.normal;
          Ray reflected_ray(result.intersection,reflected_direction);
          color_value = rayTrace(reflected_ray,rays_intersections,depth-1);
        }
      }
      return color_value;
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
