// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include "src/GLProgram.h"
#include "src/Exception.h"
#include "src/PPMIO.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#define RGB_COMPONENT_COLOR 255

#include <algorithm>
#include <GL/glut.h>

#include "src/Vec3.h"
#include "src/Camera.h"
#include "src/Scene.h"
#include "src/SelTool.h"
#include "src/Ray.h"
#include "src/matrixUtilities.h"
#include "src/Material.h"
#include "src/Instance.h"
#include "src/Texture.h"
int Instance::serial_id=0;
int Texture::bind_index_gen=0;
int Texture::bilinear_filter=1; // initialized and set here

using namespace std;

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 640;
static unsigned int SCREENHEIGHT = 480;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static unsigned int FPS = 0;
static bool fullScreen = false;

// -------------------------------------------
// Render Parametes.
// -------------------------------------------

int SPP = 32; // SamplePerPixel
float DoF = 2.5f; // depth of field
float aperture = 0.1;
int set_dof = 0;

// -------------------------------------------
// Scene Parametes.
// -------------------------------------------

Scene scene;
Vec3 inputLightPosition = Vec3(1.0,1.0,1.5);
Vec3 light_color = Vec3(1.0,1.0,1.0);
Vec3 ambient_color = Vec3(0.7,0.8,0.9);

// -------------------------------------------
// Default Material Parameters // also passed to the shader
// -------------------------------------------

float specular_intensity = 5.f;
Vec3 diffuse_color = Vec3(0.9,0.3,0.3);
Vec3 specular_color = Vec3(1.0,1.0,1.0);

// -------------------------------------------
// Shaders Parametes.
// -------------------------------------------

int lightIsInCamSpace = 0;
GLProgram *glProgram;
char *gl_program_name = "Simple GL Program";
char *vertex_shader_path = "./src/shader.vert";
char *fragment_shader_path = "./src/specular_shader.frag";

// -------------------------------------------
// Textures Parameters
// -------------------------------------------

int sphere_texture_bind_index;
int plane_texture_bind_index;
std::string sphere_texture_filename = "./src/img/scenarioTextures/scene1.ppm";
std::string plane_texture_filename = "./src/img/planeTextures/grid.ppm";

// -------------------------------------------
// Texture Checkerboard
// -------------------------------------------

unsigned char *checkerBoardImage;
unsigned int checkerBoardImageWidth = 256;
unsigned int checkerBoardImageHeight = 256;
GLuint checkerBoardImageTextudeIdx;
GLuint colorTexture_binding_Index;
std::vector<Vec3> rays_intersection;
Ray test_ray = Ray(Vec3(-1.0,0.,0.),Vec3(1.,-0.0,0.));

void setup_scene() {
  scene.set_ambient_color(ambient_color);
  scene.add_light(inputLightPosition,light_color);
  Material default_material = Material();
  default_material.set_type(DIFFUSE_SPECULAR);
  default_material.set_diffuse_color(diffuse_color);
  default_material.set_specular_color(specular_color);
  default_material.set_shininess(specular_intensity);
  scene.set_default_material(default_material);

  //scene.addMesh ("models/monkey.off");
  scene.addSphere(1.0,Vec3(0.,0.,0.));
  //scene.addSphere(0.5,Vec3(-1.1,0.,1.));
  //scene.addScenario(sphere_texture_bind_index);
  //scene.addSphere_with_texture(1.,Vec3(0,0,0),sphere_texture_bind_index);
  //scene.addSphere_with_mirror(1.0, Vec3(0.,0.,0.) );
  //scene.addCube(2,Vec3(-3.,0.,0.));
  //scene.addSphere_with_transparecy(1.0,Vec3(0.0,0.05,0.0));
  //scene.addSphere(0.7,Vec3(0.0,0.0,-1.1));
  //scene.addSphere_with_transparecy(1.0,Vec3(0.0,0.0,0.0));
  //scene.addQuad(Vec3(-10,-1,-10),Vec3(10,-1,-10),Vec3(-10,-1,10),Vec3(10,-1.0,10));
  // lb rb lt rt
  scene.addPlane_with_Texture(Vec3(-10,-1,-10),Vec3(10,-1,-10),Vec3(-10,-1,10),Vec3(10,-1.0,10),
  Vec2(0.0,0.0),Vec2(1.0,0.0),Vec2(0.0,1.0),Vec2(1.0,1.0),plane_texture_bind_index);
  //scene.addPlane_with_Texture(Vec3(5,-5,-1),Vec3(-5,-5,-1),Vec3(5,5,-1),Vec3(-5,5,-1),
  //Vec2(0.0,0.0),Vec2(1.0,0.0),Vec2(0.0,1.0),Vec2(1.0,1.0),plane_texture_bind_index);
}

void rayTraceFromCamera() {
  unsigned int w = (unsigned int) glutGet(GLUT_WINDOW_WIDTH),
	h = (unsigned int) glutGet(GLUT_WINDOW_HEIGHT);
  std::cout << "Ray tracing a " << w << " x " << h << " image" << std::endl;
  camera.apply();
  Vec3 pos , dir, dir_camera;
  std::vector< Vec3 > image( w*h , Vec3(0,0,0) );
  // calculate look at direction normalized vector
  screenSpaceToWorldSpaceRay(0.5,0.5,pos,dir_camera);
  Ray tracing_ray;
  float u,v;
  for (int y=0; y<h; y++){
    for (int x=0; x<w; x++) {
      for (int aa=0; aa<SPP; aa++) {
        std::cout << "\r" << int(((x + y*w)/float(h*w))*100) << "%";
        if (set_dof == 1) {
          u = ((float)(x) + 0.5)/ w;
          v = ((float)(y) + 0.5) / h;
          screenSpaceToWorldSpaceRay(u,v,pos,dir);
          Vec3 pos_aperture = pos + Vec3::random_unit_vector(x+y*w+h*w*aa)*aperture;
          Vec3 pos_on_focal_plane = pos+(DoF/Vec3::dot(dir_camera,dir))*dir;
          Vec3 new_dir = pos_on_focal_plane - pos_aperture;
          tracing_ray = Ray(pos_aperture, new_dir);
        }
        else {
          srand(x+y*w+h*w*aa);
          u = ((float)(x) + (float)(rand())/(float)(RAND_MAX)) / w;
          v = ((float)(y) + (float)(rand())/(float)(RAND_MAX)) / h;
          screenSpaceToWorldSpaceRay(u,v,pos,dir);
          tracing_ray = Ray(pos, dir);
        }
        image[x + y*w] += scene.rayTrace(tracing_ray, rays_intersection);
      }
      image[x + y*w] = image[x + y*w]/float(SPP);
    }
  }
  std::cout << "\n" << "done: writing image." << " \n";
  std::string filename = "./myImage.ppm";
	PPMIO::write_ppm(image,filename,w,h);
}

void createCheckerBoardImage() {
	unsigned char value;
	checkerBoardImage = new unsigned char[ 4*checkerBoardImageWidth*checkerBoardImageHeight];
	for (unsigned int i = 0; i < checkerBoardImageWidth; ++i)
		for (unsigned int j = 0; j < checkerBoardImageHeight; ++j) {
			value = 0;
			if (((2*i)/checkerBoardImageWidth+(2*j)/checkerBoardImageHeight)%2){ value = 255; }
			checkerBoardImage[4* (i*checkerBoardImageWidth+j)+0] = value;
			checkerBoardImage[4* (i*checkerBoardImageWidth+j)+1] = value;
			checkerBoardImage[4* (i*checkerBoardImageWidth+j)+2] = value;
			checkerBoardImage[4* (i*checkerBoardImageWidth+j)+3] = value;
		}
}

// -------------------------------------------

void printUsage () {
    cerr << endl
         << "gMini: a minimal OpenGL/GLUT application" << endl
         << "for 3D graphics." << endl
         << "Author : Tamy Boubekeur (http://www.labri.fr/~boubek)" << endl << endl
         << "Usage : ./gmini [<file.off>]" << endl
         << "Keyboard commands" << endl
         << "------------------" << endl
         << " ?: Print help" << endl
         << " r: Render!" << endl
         << " w: Toggle Wireframe Mode" << endl
         << " g: Toggle Gouraud Shading Mode" << endl
         << " f: Toggle full screen mode" << endl
         << " <drag>+<left button>: rotate model" << endl
         << " <drag>+<right button>: move model" << endl
         << " <drag>+<middle button>: zoom" << endl
         << " q, <esc>: Quit" << endl << endl;
}

void usage () {
    printUsage ();
    exit (EXIT_FAILURE);
}

// ------------------------------------

void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {0.5f, 1.0f, 0.5f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init () {

    sphere_texture_bind_index = scene.add_texture(sphere_texture_filename);
    plane_texture_bind_index = scene.add_texture(plane_texture_filename);
    std::cout << sphere_texture_bind_index << '\n';
    std::cout << plane_texture_bind_index << '\n';
    //Gl program

    glewExperimental = GL_TRUE;
    glewInit();
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (ambient_color[0],ambient_color[1],ambient_color[2], 1.0f);

    try {
      glProgram = GLProgram::genVFProgram (gl_program_name,vertex_shader_path,fragment_shader_path);
      glProgram -> use();
      glProgram -> setUniform1i("lightIsInCamSpace", lightIsInCamSpace);
      glProgram -> setUniform3f("diffuse_color", diffuse_color[0],diffuse_color[1],diffuse_color[2]);
      glProgram -> setUniform3f("specular_color", specular_color[0],specular_color[1],specular_color[2]);
      glProgram -> setUniform3f("ambient_color", ambient_color[0],ambient_color[1],ambient_color[2]);
      glProgram -> setUniform3f("light_color", light_color[0],light_color[1],light_color[2]);
      glProgram -> setUniform3f("inputLightPosition", inputLightPosition[0],inputLightPosition[1],inputLightPosition[2]);
      glProgram -> setUniform1f("specular_intensity", specular_intensity);

      glProgram -> stop();
    }
    catch(Exception & e) {
      cerr << e.msg() << endl;
    }
    // bind the texture
    /*
    createCheckerBoardImage();
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &checkerBoardImageTextudeIdx);
    glBindTexture(GL_TEXTURE_2D,checkerBoardImageTextudeIdx);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, checkerBoardImageWidth,
      checkerBoardImageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, checkerBoardImage);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glDisable(GL_TEXTURE_2D);
    */
}


// ------------------------------------
// Replace the code of this
// functions for cleaning memory,
// closing sockets, etc.
// ------------------------------------

void clear () {

}

// ------------------------------------
// Replace the code of this
// functions for alternative rendering.
// ------------------------------------

void drawRay(Ray ray) {
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor3f(1.,0.,0.);
  Vec3 p1 = ray.origin();
  Vec3 p2 = ray.origin()+1000*ray.direction();
  glVertex3f(p1[0],p1[1],p1[2]);
  glVertex3f(p2[0],p2[1],p2[2]);
  glEnd ();
}

void drawRay(Ray ray, float d) {
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor3f(1.,0.,0.);
  Vec3 p1 = ray.origin();
  Vec3 p2 = ray.origin()+d*ray.direction();
  glVertex3f(p1[0],p1[1],p1[2]);
  glVertex3f(p2[0],p2[1],p2[2]);
  glEnd ();
}

void drawRay(Vec3 p1, Vec3 p2) {
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor3f(1.,0.,0.);
  glVertex3f(p1[0],p1[1],p1[2]);
  glVertex3f(p2[0],p2[1],p2[2]);
  glEnd ();
  glColor3f(1.,1.,1.);
  glEnable(GL_LIGHTING);
}


void draw () {

		//std::vector<Vec3> rays_intersection;
		//scene.rayTrace(test_ray,rays_intersection);
    glEnable(GL_LIGHTING);
		if (rays_intersection.size()>0) {
			drawRay(test_ray.origin(),rays_intersection[0]);
			for (int i = 0; i<rays_intersection.size()-1; i++) {
				drawRay(rays_intersection[i],rays_intersection[i+1]);
			}
		}

  	scene.draw();

    glDisable(GL_LIGHTING);

    /*
    // Create one OpenGL texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,checkerBoardImageTextudeIdx);
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    {
      glTexCoord2f(-10,10); glVertex3f(-10,-1,10);
      glTexCoord2f(10,10); glVertex3f(10,-1,10);
      glTexCoord2f(10,-10); glVertex3f(10,-1,-10);
      glTexCoord2f(-10,-10); glVertex3f(-10,-1,-10);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    */


    //drawRay(ray,1.1);


}

void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    static float lastTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    static unsigned int counter = 0;
    counter++;
    float currentTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    if (currentTime - lastTime >= 1000.0f) {
        FPS = counter;
        counter = 0;
        static char winTitle [64];
        sprintf (winTitle, "gmini - FPS: %d", FPS);
        glutSetWindowTitle (winTitle);
        lastTime = currentTime;
    }
    glutPostRedisplay ();
}

void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
		case 'r':
			rayTraceFromCamera();
			break;
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }
        break;
    case 'q':
    case 27:
        clear ();
        exit (0);
        break;
    case 'w':
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        break;
    case 'g':
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        break;

    default:
        printUsage ();
        break;
    }
    idle ();
}

void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    onMouseClick(button,state,x,y);
    idle ();
}

void motion (int x, int y) {
    if (mouseRotatePressed == true) {
        camera.rotate (x, y);
    }
    else if (mouseMovePressed == true) {
        camera.move ((x-lastX)/static_cast<float>(SCREENWIDTH), (lastY-y)/static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true) {
        camera.zoom (float (y-lastZoom)/SCREENHEIGHT);
        lastZoom = y;
    }
}

void reshape(int w, int h) {
    camera.resize (w, h);
}

int main (int argc, char ** argv) {
    if (argc > 2) {
        printUsage ();
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("gMini");

    init ();
    setup_scene();

    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);
    scene.addGLProgram(glProgram);

    glutMainLoop ();
    return EXIT_SUCCESS;
}
