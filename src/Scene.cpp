#include "Scene.h"

int Scene::get_seed() { return ++seed%SCENE_MAX_SEED; }

RaySceneIntersection Scene::getIntersection(Ray const & ray) {
  // declare intersection objects
  RaySceneIntersection result;
  RayMeshIntersection rayMeshIntersection;
  RaySphereIntersection raySphereIntersection;
  RayLightIntersection rayLightIntersection;
  RayQuadIntersection rayQuadIntersection;

  // initialize to a max depth value before intersecting at all
  result.intersectionExists=false;
  result.lambda = SCEN_MAX_LAMBDA;

  // check quad intersection
  for(unsigned int mIt = 0 ; mIt < quads.size() ; ++mIt ) {
    rayQuadIntersection = quads[mIt].getIntersection(ray);
    if( rayQuadIntersection.intersectionExists) {
      // check if positive aka in front and near then previous interesced objects
      if (rayQuadIntersection.lambda>SCENE_MIN_LAMBDA_EPSILON && rayQuadIntersection.lambda<result.lambda) {
        result.intersectionExists = rayQuadIntersection.intersectionExists;
        result.lambda = rayQuadIntersection.lambda;
        result.intersection = rayQuadIntersection.intersection;
        result.normal = rayQuadIntersection.normal;
        result.material = rayQuadIntersection.material;
        result.objType = SCENE_OBJECT_TYPE_QUAD;
        result.objUniqueId = quads[mIt].get_unique_id();
        result.u = rayQuadIntersection.u;
        result.v = rayQuadIntersection.v;
      }
    }
  }

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
        result.u = rayMeshIntersection.u;
        result.v = rayMeshIntersection.v;

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
        result.u = rayMeshIntersection.u;
        result.v = rayMeshIntersection.v;
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
        result.u = raySphereIntersection.u;
        result.v = raySphereIntersection.v;
      }
    }
  }
  return result;
}

Vec3 Scene::rayTrace(Ray const & ray,std::vector<Vec3>& rays_intersections) {
  Vec3 color_value;
  color_value = rayTraceRecursive(ray,max_depth_recursion_path_trace);
  return color_value;
}

Vec3 Scene::rayTraceRecursive(Ray const & origin_ray, int depth) {
  // initialize color to black
  Ray ray = Ray(origin_ray.origin(), origin_ray.direction(), SCENE_RAY_OFFSET);
  Vec3 color_value = Vec3(0.,0.,0.);
  // at the end of the recursion, no contribution send back no light
  if (depth == 0) { return color_value; }

  RaySceneIntersection result = getIntersection(ray);
  // If there was no intersection return the bkg color
  if(!result.intersectionExists) { return ambient_color; }

  // PATH TRACING
  // Intersecting light
  if (result.objType == SCENE_OBJECT_TYPE_LIGHT) { return result.color; }
  // Intersecting an object
  if (result.objType == SCENE_OBJECT_TYPE_SPHERE or
    result.objType == SCENE_OBJECT_TYPE_MESH or
    result.objType == SCENE_OBJECT_TYPE_QUAD ) {

    // TRANSPARENT BRDF
    if (result.material.get_type() == TRANSPARENT) {
      Vec3 n = result.normal;
      Vec3 p = result.intersection;
      Vec3 GLASS_COLOR = result.material.get_glass_color();
      float ior = result.material.get_index_of_refraction();
      float normal_sign = 1;
      float alpha = result.material.get_alpha_mix();
      if (Vec3::dot(ray.direction(),n)>0.f) {
        ior = 1/ior;
        normal_sign = -1;
      }
      Ray refracted_ray = Ray::refracted_ray(p,ray.direction(),normal_sign*n,ior);
      Ray refl_ray = Ray::reflected_ray(ray.direction(),normal_sign*n,p);
      color_value= Vec3::componentProduct( GLASS_COLOR , alpha * rayTraceRecursive( refracted_ray, depth-1) + (1 - alpha) * rayTraceRecursive( refl_ray, depth-1) );
    }

    // MIRROR BRDF
    if (result.material.get_type() == MIRROR) {
      Vec3 TINT;
      TINT = result.material.get_tint(); //diffuse color
      Vec3 L_color;
      Vec3 p = result.intersection;
      Vec3 n = result.normal;
      // compute the reflacted ray from the intersection point p
      // with normal n
      Ray refl_ray = Ray::reflected_ray(ray.direction(),n,p);
      color_value = Vec3::componentProduct(TINT,rayTraceRecursive(refl_ray, depth-1));
    }

    // DIFFUSE SPECULAR BRDF
    if (result.material.get_type() == DIFFUSE_SPECULAR) {

      Vec3 DC;
      DC = result.material.get_diffuse_color(); //diffuse color
      if(result.material.has_texture()) {
        DC = textures[result.material.get_texture_index()].evalue(result.u,result.v);
      }
      Vec3 SC = result.material.get_specular_color(); //specular color
      float S = result.material.get_shininess(); //shininess

      Vec3 p = result.intersection;
      Vec3 n = result.normal;
      // compute the reflacted ray from the intersection point p
      // with normal n

      // BRDF part 1
      // Path tracing
      // trace a ray to every light
      for(unsigned int mIt = 0 ; mIt < lights.size() ; ++mIt ) {
        // calculate the Ray directed to the light
        Vec3 l_pos = lights[mIt].get_sample();
        Vec3 l_dir = l_pos - p;
        // angle with intersection and light source
        l_dir.normalize();
        float theta = Vec3::dot(l_dir,n);

        // DIFFUSE_SPECULAR object have no contribution from light
        // coming from behind
        if( theta > 0 ) {
          Ray l_ray = Ray(p,l_dir);
          Ray refl_l_ray = Ray::reflected_ray(p,l_dir,n);
          RaySceneIntersection result = getIntersection(l_ray);
          if (result.intersectionExists) {
            // check if interected the current light
            if (result.objUniqueId == lights[mIt].get_unique_id()) {
              Vec3 L_color = lights[mIt].get_color();
              // a particular case. a ray may intersect the light
              // but the normal to the intersection is opposite to the ray
              // so check befor intersecting to avoid lounching
              float solid_angle = lights[mIt].solid_angle(p);
              float refl_to_eye_angle = std::max(0.f,Vec3::dot(refl_l_ray.direction(),-1*ray.direction()));
              float sigma = std::pow(refl_to_eye_angle,S);
              color_value += solid_angle * Vec3::componentProduct(
                L_color,
                DC*theta + SC*sigma// DIFFUSE + SPECULAR NO ambient in path thracing
              );
            }
          }
        }

      }

      // DIFFUSE SPECULAR BRDF part 2
      // Montecarlo sampling the global illumination part
      Ray random_ray = Ray(p,Vec3::random_in_emisphere(p,n,get_seed()) );
      Ray reflected_random_ray = Ray::reflected_ray(p,random_ray.direction(),n);
      float sigma_1 = Vec3::dot(n,random_ray.direction());
      float sigma_2 = std::max(0.f,Vec3::dot(reflected_random_ray.direction(),-1*ray.direction()));
      color_value += Vec3::componentProduct(
        sigma_1*DC + std::pow(sigma_2,S)*SC,
        rayTraceRecursive(random_ray, depth-1)
      );
      return color_value;
    }
  }

  return color_value;
}

int Scene::add_texture(std::string & filename) {
  //load Textures
  Texture texture = Texture();
  texture.load_texture(filename);
  textures.push_back(texture);
  return texture.get_bindIndex();
}

void Scene::addPlane_with_Texture(Vec3 c1, Vec3 c2, Vec3 c3, Vec3 c4, Vec2 uvc1, Vec2 uvc2, Vec2 uvc3, Vec2 uvc4, int bind_index_texture){
  Quad quad = Quad();
  quad.set_material(default_material);
  quad.set_texture_index(bind_index_texture);
  quad.bindindex = bind_index_texture;
  quad.setQuads_with_text(c1,c2,c3,c4,uvc1,uvc2,uvc3,uvc4);
  quad.recomputeNormals ();
  quad.buildArray();
  quads.push_back(quad);
}

void Scene::addSphere_with_mirror(float _ray, Vec3 _center ) {
  Sphere sphere = Sphere(_ray,_center);
  sphere.buildMesh(15,15);
  sphere.material.set_mirror();
  spheres.push_back(sphere);
}

void Scene::addSphere_with_transparecy(float _ray, Vec3 _center ) {
  Sphere sphere = Sphere(_ray,_center);
  sphere.buildMesh(15,15);
  sphere.material.set_tranparent(0.7,1.0);
  spheres.push_back(sphere);
}

void Scene::addSphere_with_texture(float _ray, Vec3 _center ,int bind_index_texture) {
  Sphere sphere = Sphere(_ray,_center);
  sphere.set_material(default_material);
  sphere.buildMesh(15,15);
  sphere.set_texture_index(bind_index_texture);
  sphere.bindindex = bind_index_texture;
  spheres.push_back(sphere);
}

void Scene::addSphere(float _ray, Vec3 _center ) {
  Sphere sphere = Sphere(_ray,_center);
  sphere.set_material(default_material);
  sphere.buildMesh(15,15);
  spheres.push_back(sphere);
}

void Scene::addCube(float _side, Vec3 _center ) {
  Cube cube = Cube(_side,_center);
  cube.set_material(default_material);
  cube.buildMesh(4);
  cubes.push_back(cube);
}

void Scene::addQuad(Vec3 c1, Vec3 c2, Vec3 c3, Vec3 c4) {
  meshes.resize( meshes.size() + 1 );
  Mesh & meshAjoute = meshes[ meshes.size() - 1 ];
  meshAjoute.set_material(default_material);
  meshAjoute.setQuads(c1,c2,c3,c4);
  //meshAjoute.centerAndScaleToUnit ();
  meshAjoute.recomputeNormals ();
  meshAjoute.buildArray();
}

void Scene::addQuad_with_texture(Vec3 c1, Vec3 c2, Vec3 c3, Vec3 c4, Vec2 uvc1, Vec2 uvc2, Vec2 uvc3, Vec2 uvc4, int bind_index_texture) {
  meshes.resize( meshes.size() + 1 );
  Mesh & meshAjoute = meshes[ meshes.size() - 1 ];
  meshAjoute.set_material(default_material);
  meshAjoute.setQuads_with_text(c1,c2,c3,c4,uvc1,uvc2,uvc3,uvc4);
  meshAjoute.set_texture_index(bind_index_texture);
  //meshAjoute.centerAndScaleToUnit ();
  meshAjoute.recomputeNormals ();
  meshAjoute.buildArray();
}

void Scene::addMesh(std::string const & modelFilename) {
  meshes.resize( meshes.size() + 1 );
  Mesh & meshAjoute = meshes[ meshes.size() - 1 ];
  meshAjoute.set_material(default_material);
  meshAjoute.loadOFF (modelFilename);
  meshAjoute.centerAndScaleToUnit ();
  meshAjoute.recomputeNormals ();
  meshAjoute.buildArray();
}

void Scene::add_light(Vec3 const & light_position) {
  Light light = Light(light_position);
  lights.push_back(light);
}

void Scene::add_light(Vec3 const & light_position, Vec3 const & light_color ) {
  Light light = Light(light_position,light_color);
  lights.push_back(light);
}

void Scene::addGLProgram(GLProgram* _gl_Program) {
  gl_Program = _gl_Program;
}

void Scene::draw() const {
    // iterer sur l'ensemble des objets, et faire leur rendu.
  for( unsigned int mIt = 0 ; mIt < meshes.size() ; ++mIt ) {
      Mesh const & mesh = meshes[mIt];
      //std::cout << "mesh bindindex" << textures[mesh.bindindex].get_bindIndex() << '\n';
      //std::cout << "mesh bindindex" << mesh.bindindex << '\n';
      //std::cout << "mesh handleIndex" << textures[mesh.bindindex].get_handleIndex() << '\n';
      gl_Program->use();
      gl_Program -> setUniform1i("has_texture", GLuint(mesh.has_texture));
      if (mesh.has_texture == 1) {
        //std::cout << "mesh get_bindIndex" <<  textures[mesh.bindindex].get_bindIndex() << '\n';
        //std::cout << "mesh get_handleIndex" <<  textures[mesh.bindindex].get_handleIndex() << '\n';
        gl_Program->setUniform1i("uTextureColor", textures[mesh.bindindex].get_bindIndex());
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GLuint(GL_TEXTURE0+textures[mesh.bindindex].get_bindIndex()));
        glBindTexture(GL_TEXTURE_2D , textures[mesh.bindindex].get_handleIndex());
        glDisable(GL_LIGHTING);
        glColor3f(1.,1.,1.);
        mesh.drawWithTexture();
        glDisable(GL_TEXTURE_2D);
      }
      else
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
  for( unsigned int mIt = 0 ; mIt < quads.size() ; ++mIt ) {
      Quad const & quad = quads[mIt];
      gl_Program->use();
      gl_Program -> setUniform1i("has_texture", GLuint(quad.has_texture));
      if (quad.has_texture == 1) {
        gl_Program->setUniform1i("uTextureColor", textures[quad.bindindex].get_bindIndex());
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GLuint(GL_TEXTURE0+textures[quad.bindindex].get_bindIndex()));
        glBindTexture(GL_TEXTURE_2D , textures[quad.bindindex].get_handleIndex());
        glDisable(GL_LIGHTING);
        glColor3f(1.,1.,1.);
        quad.drawWithTexture();
        glDisable(GL_TEXTURE_2D);
      }
      else
        quad.draw();
      gl_Program->stop();
  }

  for( unsigned int mIt = 0 ; mIt < cubes.size() ; ++mIt ) {
      Cube const & cube = cubes[mIt];
      gl_Program->use();
      gl_Program -> setUniform1i("has_texture", GLuint(cube.has_texture));
      if (cube.has_texture == 1) {
        gl_Program->setUniform1i("uTextureColor", textures[cube.bindindex].get_bindIndex());
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GLuint(GL_TEXTURE0+textures[cube.bindindex].get_bindIndex()));
        glBindTexture(GL_TEXTURE_2D , textures[cube.bindindex].get_handleIndex());
        glDisable(GL_LIGHTING);
        glColor3f(1.,1.,1.);
        cube.drawWithTexture();
        glDisable(GL_TEXTURE_2D);
      }
      else
        cube.draw();
      gl_Program->stop();
  }
  for( unsigned int mIt = 0 ; mIt < spheres.size() ; ++mIt ) {
      Sphere const & sphere = spheres[mIt];
      gl_Program->use();
      gl_Program -> setUniform1i("has_texture", GLuint(sphere.has_texture));

      if (sphere.has_texture == 1) {
        //std::cout << "sphere get_bindIndex" <<  textures[sphere.bindindex].get_bindIndex() << '\n';
        //std::cout << "sphere get_handleIndex" <<  textures[sphere.bindindex].get_handleIndex() << '\n';

        gl_Program->setUniform1i("uTextureColor", textures[sphere.bindindex].get_bindIndex());
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GLuint(GL_TEXTURE0+textures[sphere.bindindex].get_bindIndex()));
        glBindTexture(GL_TEXTURE_2D , textures[sphere.bindindex].get_handleIndex());
        glDisable(GL_LIGHTING);
        glColor3f(1.,1.,1.);
        sphere.drawWithTexture();
        glDisable(GL_TEXTURE_2D);
      }
      else
        sphere.draw();
      gl_Program->stop();
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
