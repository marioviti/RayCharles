#ifndef MATERIAL_H
#define MATERIAL_H

#define DIFFUSE 0
#define SPECULAR 1
#define DIFFUSE_SPECULAR 2
#define MIRROR 3
#define TRANSPARENT 4

#include "Vec3.h"
#include "Texture.h"

class Material {
  private:
    int type;
    bool texturized;
    bool mirror;
    Vec3 tint;
    Vec3 diffuse_color;
    Vec3 specular_color;
    float shininess;
    float index_of_refraction;
    float alpha_mix;

    int texture_bind_index;

  public:
    Material() {
        type=DIFFUSE_SPECULAR;
        specular_color=Vec3(1,1,1);
        diffuse_color=Vec3(0.3,0.4,0.5);
        shininess=3.3;
        texturized = false;
        mirror=false;
    }
    bool has_texture() {return texturized;}
    void set_texture_index(int texture_bind_index_) { texture_bind_index = texture_bind_index_; texturized = true;}
    void unset_texture() { texturized=false; }
    int get_texture_index() { return texture_bind_index; }

    bool is_mirror() { return mirror; }
    void set_mirror() { tint=Vec3(0.,0.,0);  mirror=true; type = MIRROR; }
    void unset_mirror() { mirror=false; }

    void set_tranparent() { alpha_mix=0.75; index_of_refraction=1.1; type = TRANSPARENT; }
    void set_tranparent(float ior) { index_of_refraction=ior; alpha_mix=0.75; type = TRANSPARENT; }
    void set_tranparent(float ior, float alpha) { index_of_refraction=ior; alpha_mix=alpha; type = TRANSPARENT; }
    float get_index_of_refraction() { return index_of_refraction; }
    float get_alpha_mix() { return alpha_mix; }

    void set_tint(Vec3 _tint) { tint = _tint; }
    Vec3 get_tint() { return tint; }

    void set_diffuse_color(Vec3 _color) { diffuse_color = _color; }
    Vec3 get_diffuse_color() { return diffuse_color; }

    void set_specular_color(Vec3 _color) { specular_color = _color; }
    Vec3 get_specular_color() { return specular_color; }

    void set_shininess(float _shininess) { shininess = _shininess; }
    float get_shininess() {return shininess;}

    void set_type(int _type) { type = _type; }
    int get_type() { return type; }

    void set_texture();

    static Material default_material() {
      return Material();
    }
};

#endif
