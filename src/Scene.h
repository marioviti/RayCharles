#ifndef SCENE_H
#define SCENE_H

#define SCENE_MAX_SEED 10000000
#define SCEN_MAX_LAMBDA 1000
#define SCENE_RAY_OFFSET 0.001f
#define SCENE_MIN_LAMBDA_EPSILON 0.001f
#define SCENE_OBJECT_TYPE_LIGHT 0
#define SCENE_OBJECT_TYPE_SPHERE 1
#define SCENE_OBJECT_TYPE_MESH 2

#include <vector>
#include <string>
#include "Mesh.h"
#include "Sphere.h"
#include "Ray.h"
#include "Light.h"
#include "Cube.h"

#include <GL/glut.h>

struct RaySceneIntersection {
  int objType;
  int objUniqueId;
  Material material;
  bool intersectionExists;
  float lambda;
  Vec3 intersection;
  Vec3 normal;
  Vec3 color;
};

class Scene {

    // Mettez ici tout ce que vous souhaitez avoir dans votre scene 3D.
    // Pour l'instant, on a uniquement des maillages, mais par la suite on pourra rajouter des objets specialises comme des spheres, des cylindres ou des cones par ex...

private:

    std::vector<Mesh> meshes;
    std::vector<Sphere> spheres;
    std::vector<Cube> cubes;
    std::vector<Light> lights;

    int seed; //use to initialize random functions
    int max_depth_recursion_path_trace;

    GLProgram* gl_Program;
    Vec3 bkg_color;

public:
    Scene() { bkg_color = Vec3(0.1,0.2,0.6); max_depth_recursion_path_trace=10; }

    // Rendering methods
    int get_seed();
    RaySceneIntersection getIntersection(Ray const & ray);
    Vec3 rayTrace(Ray const & ray,std::vector<Vec3>& rays_intersections);
    Vec3 rayTraceRecursive(Ray const & ray, std::vector<Vec3>& rays_intersections, int depth);

    // OBJECT ADD methods
    void addSphere(float _ray, Vec3 _center );
    void addCube(float _side, Vec3 _center );
    void addQuad(Vec3 c1, Vec3 c2, Vec3 c3, Vec3 c4);
    void addMesh(std::string const & modelFilename);
    void add_light(Vec3 const & light_position);

    // ADD SHADERS as GLPROGRAM class
    void addGLProgram(GLProgram* _gl_Program);
    void draw() const;
};

#endif
