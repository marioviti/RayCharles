

#include "Texture.h"
#include "PPMIO.h"

int Texture::new_bind_idx() {
  return Texture::bind_index_gen++; // da implementare ammodino con un counter
}

Vec3 Texture::evalue(float u, float v) {
  float pu,pv,r,g,b,du,dv;
  int nu,nv;
  pu = u*w; pv = v*h;
  nu = int(floor(pu)); nv = int(floor(pv));
  du = pu-nu-0.5; dv = pv-nv-0.5;

  // Linear interpolation
  r = (1-du)*(1-dv)*imageTexture[3*(nu +w*nv)+0];
  r += (1-du)*dv*imageTexture[3*(nu +w*(nv+1))+0];
  r += du*(1-dv)*imageTexture[3*(nu+1 +w*(nv))+0];
  r += du*dv*imageTexture[3*(nu+1 +w*(nv+1))+0];

  g = (1-du)*(1-dv)*imageTexture[3*(nu +w*nv)+1];
  g += (1-du)*dv*imageTexture[3*(nu +w*(nv+1))+1];
  g += du*(1-dv)*imageTexture[3*(nu+1 +w*(nv))+1];
  g += du*dv*imageTexture[3*(nu+1 +w*(nv+1))+1];

  b = (1-du)*(1-dv)*imageTexture[3*(nu +w*nv)+2];
  b += (1-du)*dv*imageTexture[3*(nu +w*(nv+1))+2];
  b += du*(1-dv)*imageTexture[3*(nu+1 +w*(nv))+2];
  b += du*dv*imageTexture[3*(nu+1 +w*(nv+1))+2];
  return Vec3(r/255.f,g/255.f,b/255.f);
}

void Texture::load_texture(std::string& filename) {
  unsigned int H,W;
  PPMIO::load_ppm(imageTexture,W,H,filename,PPMIO::rgb);
  h = H; w = W;
  bindIndex = new_bind_idx();
  glEnable (GL_TEXTURE_2D);
  glGenTextures (bindIndex, &handleIndex);
  glBindTexture (GL_TEXTURE_2D , handleIndex);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGB , w , h , 0 , GL_RGB , GL_UNSIGNED_BYTE , imageTexture);
  glDisable(GL_TEXTURE_2D);
}
