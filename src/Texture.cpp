
 /*
 #include "Texture.h"
#include "PPMIO.h"

int Texture::new_bind_idx() {
  return 0; // da implementare ammodino con un counter
}

void Texture::load_texture(std::string& filename) {
  unsigned int w;
  unsigned int h;
  unsigned char *image;
  PPMIO::load_ppm(image,w,h,filename,PPMIO::rgb);
  colorTexture_binding_Index = new_bind_idx();
  glGenTextures(GL_TEXTURE0 + colorTexture_binding_Index, &colorTexture_handler_Index);
  std::cout << colorTexture_handler_Index << colorTexture_binding_Index << '\n';
}
*/
