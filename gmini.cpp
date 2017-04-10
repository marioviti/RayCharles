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
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <GL/glut.h>

#include "src/Vec3.h"
#include "src/Camera.h"
#include "src/Scene.h"
#include "src/SelTool.h"
#include "src/Ray.h"

#include "src/matrixUtilities.h"

#include "src/Instance.h"
int Instance::serial_id=0;

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
// Shaders Parametes.
// -------------------------------------------
Scene scene;

static Vec3 inputLightPosition = Vec3(-1.0, -1.0, 0.0);
int lightIsInCamSpace = 1;
float specular_intensity = 3.3;
GLProgram *glProgram;
char *gl_program_name = "Simple GL Program";
char *vertex_shader_path = "./src/shader.vert";
char *fragment_shader_path = "./src/specular_shader.frag";

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

/*

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
            image[x + y*w] = scene.rayTrace( Ray(pos, dir), rays_intersection);
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
        f << (int)(255.f*image[i][0]) << " " << (unsigned char)(255.f*image[i][1]) << " " << (unsigned char)(255.f*image[i][2]) << " ";
    f << std::endl;
    f.close();
}

*/

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
    glewExperimental = GL_TRUE;
    glewInit();
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);

    try {
      glProgram = GLProgram::genVFProgram (gl_program_name,vertex_shader_path,fragment_shader_path);
      glProgram -> use();
      glProgram -> setUniform1i("lightIsInCamSpace", lightIsInCamSpace);
      glProgram -> setUniform1f("specular_intensity", specular_intensity);
      glProgram -> setUniform3f("inputLightPosition", inputLightPosition[0],inputLightPosition[1],inputLightPosition[2]);
      glProgram -> stop();
    }
    catch(Exception & e) {
      cerr << e.msg() << endl;
    }
    // bind the texture
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
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);
    scene.addMesh (argc == 2 ? argv[1] : "models/monkey.off");
    scene.addGLProgram(glProgram); //ADD ONE PER MESH!!!!!!
    scene.add_light(inputLightPosition);
		//scene.addSphere(0.4,Vec3(0.,0.,0.));
    scene.addSphere(0.3,Vec3(-1.0,1.0,1.0));
    scene.addQuad(Vec3(-10,-1,-10),Vec3(10,-1,-10),Vec3(-10,-1,10),Vec3(10,-1.0,10));

		// RAY TRACER

    scene.rayTrace(test_ray,rays_intersection);

    glutMainLoop ();
    return EXIT_SUCCESS;
}
