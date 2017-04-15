#include "Scene.h"
#include "Sphere.h"
#include "Mesh.h"

int Scene::get_seed() { return ++seed%SCENE_MAX_SEED; }

RaySceneIntersection Scene::getIntersection(Ray const & ray) {
  // declare intersection objects
  RaySceneIntersection result;
  RayMeshIntersection rayMeshIntersection;
  RaySphereIntersection raySphereIntersection;
  RayLightIntersection rayLightIntersection;

  // initialize to a max depth value before intersecting at all
  result.intersectionExists=false;
  result.lambda = SCEN_MAX_LAMBDA;

  // check Light intersection
  for(unsigned int mIt = 0 ; mIt < lights.size() ; ++mIt ) {
    rayLightIntersection = lights[mIt].getIntersection(ray);
    if( rayLightIntersection.intersectionExists) {
      // check if positive aka in front and near then previous interesced objects
      if (rayLightIntersection.lambda>SCENE_MIN_LAMBDA_EPSILON && rayLightIntersection.lambda<result.lambda) {

        result.intersectionExists = rayLightIntersection.intersectionExists;
        result.lambda = rayLightIntersection.lambda;
        result.intersection = rayLightIntersection.intersection;
        result.normal = rayLightIntersection.normal;
        result.color = rayLightIntersection.color;
        result.objType = SCENE_OBJECT_TYPE_LIGHT;
        result.objUniqueId = lights[mIt].get_unique_id();

      }
    }
  }

  // check Cubes intersection
  for(unsigned int mIt = 0 ; mIt < cubes.size() ; ++mIt ) {
    rayMeshIntersection = cubes[mIt].getIntersection(ray);
    if(rayMeshIntersection.intersectionExists)
      // check if positive aka in front and near then previous interesced objects
      if (rayMeshIntersection.lambda>SCENE_MIN_LAMBDA_EPSILON && rayMeshIntersection.lambda<result.lambda) {
        result.intersectionExists = rayMeshIntersection.intersectionExists;
        result.lambda = rayMeshIntersection.lambda;
        result.intersection = rayMeshIntersection.intersection;
        result.normal = rayMeshIntersection.normal;
        result.material = rayMeshIntersection.material;
        result.objType = SCENE_OBJECT_TYPE_MESH;
        result.objUniqueId = cubes[mIt].get_unique_id();

        //std::cout << '\n' << "intersection" << '\n';
        //std::cout << "intersection " << result.intersection << '\n';
        //std::cout << "lambda " << result.lambda << '\n';
        //std::cout << "normal " << result.normal << '\n';
      }
  }

  // check Meshes intersection
  for(unsigned int mIt = 0 ; mIt < meshes.size() ; ++mIt ) {
    rayMeshIntersection = meshes[mIt].getIntersection(ray);
    if(rayMeshIntersection.intersectionExists)
      // check if positive aka in front and near then previous interesced objects
      if (rayMeshIntersection.lambda>SCENE_MIN_LAMBDA_EPSILON && rayMeshIntersection.lambda<result.lambda) {
        result.intersectionExists = rayMeshIntersection.intersectionExists;
        result.lambda = rayMeshIntersection.lambda;
        result.intersection = rayMeshIntersection.intersection;
        result.normal = rayMeshIntersection.normal;
        result.material = rayMeshIntersection.material;
        result.objType = SCENE_OBJECT_TYPE_MESH;
        result.objUniqueId = meshes[mIt].get_unique_id();

        //std::cout << '\n' << "intersection" << '\n';
        //std::cout << "intersection " << result.intersection << '\n';
        //std::cout << "lambda " << result.lambda << '\n';
        //std::cout << "normal " << result.normal << '\n';
      }
  }

  // check Spheres intersection
  for(unsigned int mIt = 0 ; mIt < spheres.size() ; ++mIt ) {
    raySphereIntersection = spheres[mIt].getIntersection(ray);
    if(raySphereIntersection.intersectionExists){
      // check if positive aka in front and near then previous interesced objects
      if (raySphereIntersection.lambda>SCENE_MIN_LAMBDA_EPSILON && raySphereIntersection.lambda<result.lambda) {
        result.intersectionExists = raySphereIntersection.intersectionExists;
        result.lambda = raySphereIntersection.lambda;
        result.intersection = raySphereIntersection.intersection;
        result.normal = raySphereIntersection.normal;
        result.material = raySphereIntersection.material;
        result.objType = SCENE_OBJECT_TYPE_SPHERE;
        result.objUniqueId = spheres[mIt].get_unique_id();
      }
    }
  }
  return result;
}

Vec3 Scene::rayTrace(Ray const & ray,std::vector<Vec3>& rays_intersections) {
  Vec3 color_value;
  color_value = rayTraceRecursive(ray,rays_intersections,max_depth_recursion_path_trace);
  return color_value;
}

Vec3 Scene::rayTraceRecursive(Ray const & ray, std::vector<Vec3>& rays_intersections, int depth) {
  // initialize color to black
  Vec3 color_value = Vec3(0.,0.,0.);
  // at the end of the recursion, no contribution send back no light
  if (depth == 0) { return color_value; }

  RaySceneIntersection result = getIntersection(ray);
  // If there was no intersection return the bkg color
  if(!result.intersectionExists) { return bkg_color; }

  // TESTS
  //rays_intersections.push_back(result.intersection);
  // End TESTS

  // PATH TRACING
  // Intersecting light
  if (result.objType == SCENE_OBJECT_TYPE_LIGHT) { return result.color; }
  // Intersecting an object
  if (result.objType == SCENE_OBJECT_TYPE_SPHERE or
    result.objType == SCENE_OBJECT_TYPE_MESH ) {
    // DIFFUSE BRDF
    if (result.material.get_type() == DIFFUSE_SPECULAR) {

      //std::cout << '\n' << "intersection in BRDF" << '\n';
      //std::cout << "intersection " << result.intersection << '\n';
      //std::cout << "lambda " << result.lambda << '\n';
      //std::cout << "normal " << result.normal << '\n';

      Vec3 DC = result.material.get_diffuse_color(); //diffuse color
      Vec3 SC = result.material.get_specular_color(); //specular color
      float S = result.material.get_shininess(); //shininess

      Vec3 p = result.intersection;
      Vec3 n = result.normal;
      // compute the reflacted ray from the intersection point p
      // with normal n
      Ray refl_ray = Ray::reflected_ray(ray.direction(),n,p);
      // BRDF part 1
      // Path tracing
      // trace a ray to every light
      for(unsigned int mIt = 0 ; mIt < lights.size() ; ++mIt ) {
        // calculate the Ray directed to the light
        Vec3 l_pos = lights[mIt].get_sample();
        Vec3 l_dir = l_pos - p;
        // angle with intersection and light source
        float theta_1 = Vec3::dot(l_dir,n);

        // DIFFUSE_SPECULAR object have no contribution from light
        // coming from behind
        if( theta_1 > 0 ) {
          Ray l_ray = Ray(p,l_dir,SCENE_RAY_OFFSET);
          // calculate omega in
          RaySceneIntersection result = getIntersection(l_ray);
          if (result.intersectionExists) {
            // check if interected the current light
            if (result.objUniqueId == lights[mIt].get_unique_id()) {
              Vec3 L_color = lights[mIt].get_color();
              // a particular case. a ray may intersect the light
              // but the normal to the intersection is opposite to the ray
              // so check befor intersecting to avoid lounching
              float solid_angle = lights[mIt].solid_angle(p);
              float theta_2 = std::max(0.f,Vec3::dot(refl_ray.direction(),-1*ray.direction()));
              color_value += solid_angle * Vec3::componentProduct(
                theta_1*DC + SC*std::pow(theta_2,S), // DIFFUSE + SPECULAR
                L_color // Light color
              );
            }
          }
        }
      }

      // BRDF part 2
      // Montecarlo sampling
      Ray random_ray = Ray(p,Vec3::random_in_emisphere(p,n,get_seed()),SCENE_RAY_OFFSET);
      Ray reflected_random_ray = Ray::reflected_ray(random_ray.direction(),n,p);
      float solid_angle = 1.f;
      float sigma_1 = Vec3::dot(n,random_ray.direction());
      float sigma_2 = std::max(0.f,Vec3::dot(reflected_random_ray.direction(),-1*ray.direction()));
      color_value += solid_angle * Vec3::componentProduct(
        sigma_1*DC + SC*std::pow(sigma_2,S),
        rayTraceRecursive(random_ray,rays_intersections,depth-1)
      );
      return color_value;
    }
  }

  return color_value;
}

void Scene::addSphere(float _ray, Vec3 _center ) {
  Sphere sphere = Sphere(_ray,_center);
  sphere.buildMesh(15,15);
  spheres.push_back(sphere);
}

void Scene::addCube(float _side, Vec3 _center ) {
  Cube cube = Cube(_side,_center);
  cube.buildMesh(4);
  cubes.push_back(cube);
}

void Scene::addQuad(Vec3 c1, Vec3 c2, Vec3 c3, Vec3 c4) {
  meshes.resize( meshes.size() + 1 );
  Mesh & meshAjoute = meshes[ meshes.size() - 1 ];
  meshAjoute.setQuads(c1,c2,c3,c4);
  //meshAjoute.centerAndScaleToUnit ();
  meshAjoute.recomputeNormals ();
  meshAjoute.buildArray();
}

void Scene::addMesh(std::string const & modelFilename) {
  meshes.resize( meshes.size() + 1 );
  Mesh & meshAjoute = meshes[ meshes.size() - 1 ];
  meshAjoute.loadOFF (modelFilename);
  meshAjoute.centerAndScaleToUnit ();
  meshAjoute.recomputeNormals ();
  meshAjoute.buildArray();
}

void Scene::add_light(Vec3 const & light_position) {
  Light light = Light(light_position);
  lights.push_back(light);
}

void Scene::addGLProgram(GLProgram* _gl_Program) {
  gl_Program = _gl_Program;
}

void Scene::draw() const {
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
  for( unsigned int mIt = 0 ; mIt < cubes.size() ; ++mIt ) {
      Cube const & cube = cubes[mIt];
      cube.draw();
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
