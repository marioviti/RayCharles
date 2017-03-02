

void rayTraceFromCamera() {
    int w = glutGet(GLUT_WINDOW_WIDTH)  ,   h = glutGet(GLUT_WINDOW_HEIGHT);
    std::cout << "Ray tracing a " << w << " x " << h << " image" << std::endl;
    camera.apply();
    Vec3 pos , dir;
    std::vector< Vec3 > image( w*h , Vec3(0,0,0) );
    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++) {
            float u = ((float)(x) + (float)(rand())/(float)(RAND_MAX)) / w;
            float v = ((float)(y) + (float)(rand())/(float)(RAND_MAX)) / h;
            // this is a random uv that belongs to the pixel xy.
            screenSpaceToWorldSpaceRay(u,v,pos,dir);
            image[x + y*w] = scene.rayTrace( Ray(pos , dir) , rays );
        }
    }

    std::string filename = "./myImage.ppm";
    ofstream f(filename.c_str(), ios::binary);
    if (f.fail()) {
        cout << "Could not open file: " << filename << endl;
        return;
    }
    f << "P3" << std::endl << w << " " << h << std::endl << 255 << std::endl;
    for (int i=0; i<w*h; i++)
        f << (unsigned char)(255.f*image[i][0]) << " " << (unsigned char)(255.f*image[i][1]) << " " << (unsigned char)(255.f*image[i][2]) << " ";
    f << std::endl;
    f.close();
}
