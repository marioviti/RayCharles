#ifndef SCENE_H
#define SCENE_H

#define SCENE_MAX_SEED 1000000
#define SCEN_MAX_LAMBDA 1000000
#define SCENE_RAY_OFFSET 10e-6f
#define SCENE_MIN_LAMBDA_EPSILON 10e-6f
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
  float u,v;
  float lambda;
  Vec3 intersection;
  Vec3 normal;
  Vec3 color;
};

class Scene {

    // Mettez ici tout ce que vous souhaitez avoir dans votre scene 3D.
    // Pour l'instant, on a uniquement des maillages, mais par la suite on pourra rajouter des objets specialises comme des spheres, des cylindres ou des cones par ex...

private:

    std::vector<Texture> textures;
    std::vector<Mesh> meshes;
    std::vector<Sphere> spheres;
    std::vector<Cube> cubes;
    std::vector<Light> lights;

    int seed; //use to initialize random functions
    int max_depth_recursion_path_trace;

    GLProgram* gl_Program;
    Vec3 ambient_color;
    Material default_material;

public:
    Scene() { ambient_color = Vec3(0.5,0.5,0.5); max_depth_recursion_path_trace=8; }

    // getter and setters
    Material get_default_material() { return default_material; }
    void set_default_material(Material def_material) { default_material = def_material; }

    // Rendering methods
    int get_seed();
    RaySceneIntersection getIntersection(Ray const & ray);
    Vec3 rayTrace(Ray const & ray,std::vector<Vec3>& rays_intersections);
    Vec3 rayTraceRecursive(Ray const & ray, std::vector<Vec3>& rays_intersections, int depth);
    int add_texture(std::string & filename);

    // OBJECT ADD methods
    void set_ambient_color(Vec3 ambient_color_) { ambient_color=ambient_color_; };
    void addSphere(float _ray, Vec3 _center );
    void addSphere_with_texture(float _ray,  Vec3 _center, int bind_index_texture);
    void addSphere_with_mirror(float _ray, Vec3 _center );
    void addSphere_with_transparecy(float _ray, Vec3 _center );

    void addCube(float _side, Vec3 _center );
    void addCube(float _side, Vec3 _center, Vec3 diffuse_color);
    void addQuad(Vec3 c1, Vec3 c2, Vec3 c3, Vec3 c4);
    void addMesh(std::string const & modelFilename);
    void add_light(Vec3 const & light_position);
    void add_light(Vec3 const & light_position, Vec3 const & light_color );


    // ADD SHADERS as GLPROGRAM class
    void addGLProgram(GLProgram* _gl_Program);
    void draw() const;
};

#endif
