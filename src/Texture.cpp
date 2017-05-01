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

  // AA bilinera filtering
  if (Texture::bilinear_filter!=1) {
    r = imageTexture[3*(nu +w*nv)+0];
    g = imageTexture[3*(nu +w*(nv))+1];
    b = imageTexture[3*(nu +w*(nv))+2];
    return Vec3(r/255.f,g/255.f,b/255.f);
  }
  if ( nu==0 or nu == w-1 or nv == 0 or nv == h-1 ) {
    r = imageTexture[3*(nu +w*nv)+0];
    g = imageTexture[3*(nu +w*(nv))+1];
    b = imageTexture[3*(nu +w*(nv))+2];
  }
  /*
  if (( nu==0 or nu == w-1) and (nv == 0 or nv == h-1) ) {
    // no interpolation
    r = imageTexture[3*(nu +w*nv)+0];
    g = imageTexture[3*(nu +w*(nv))+1];
    b = imageTexture[3*(nu +w*(nv))+2];
  }
  else if ( nu==0 or nu == w-1) {
    // linera interpolation vertical
    r = (1-dv)*imageTexture[3*(nu +w*nv)+0];
    r += (dv)*imageTexture[3*(nu +w*nv+1)+0];

    g = (1-dv)*imageTexture[3*(nu +w*nv)+1];
    g += (dv)*imageTexture[3*(nu +w*nv+1)+1];

    b = (1-dv)*imageTexture[3*(nu +w*nv)+1];
    b += (dv)*imageTexture[3*(nu +w*nv+1)+1];

  }
  else if ( nv == 0 or nv == h-1 ) {
    // linera interpolation horizontal
    r = (1-du)*imageTexture[3*(nu +w*nv)+0];
    r += (du)*imageTexture[3*(nu+1 +w*nv)+0];

    g = (1-du)*imageTexture[3*(nu +w*nv)+1];
    g += (du)*imageTexture[3*(nu+1 +w*nv)+1];

    b = (1-du)*imageTexture[3*(nu +w*nv)+2];
    b += (du)*imageTexture[3*(nu+1 +w*nv)+2];
  }
  */
  else {
    // bilinear interpolation
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
  }

  // check for little errors
  if(r>255.f) { r=255.f;}
  if(g>255.f) { g=255.f;}
  if(b>255.f) { b=255.f;}

  if(r<0.f) { r=0.f;}
  if(g<0.f) { g=0.f;}
  if(b<0.f) { b=0.f;}

  return Vec3(r/255.f,g/255.f,b/255.f);
}

void Texture::load_texture(std::string& filename) {
  unsigned int H,W;
  PPMIO::load_ppm(imageTexture,W,H,filename,PPMIO::rgb);
  h = H; w = W;
  bindIndex = new_bind_idx();
  glEnable (GL_TEXTURE_2D);
  glGenTextures (bindIndex, &handleIndex);
  std::cout << "texture "<< bindIndex << "handle " << handleIndex << '\n';
  glBindTexture (GL_TEXTURE_2D , handleIndex);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGB , w , h , 0 , GL_RGB , GL_UNSIGNED_BYTE , imageTexture);
  glDisable(GL_TEXTURE_2D);
}
