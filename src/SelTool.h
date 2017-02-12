#ifndef SEL_TOOL_H
#define SEL_TOOL_H

#include <GL/glut.h>
#include <stdio.h>
#include <glm/glm.hpp>

void onMouseClick(int button, int state, int x, int y) {
  printf("called");
  if(state != GLUT_DOWN)
    return;

  int window_width = glutGet(GLUT_WINDOW_WIDTH);
  int window_height = glutGet(GLUT_WINDOW_HEIGHT);

  GLbyte color[4];
  GLfloat depth;
  GLuint index;

  glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
  glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
  glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

  GLfloat projection[16];
  glGetFloatv (GL_MODELVIEW_MATRIX, projection);

  glm::vec4 viewport = glm::vec4(0, 0, window_width, window_height);
  glm::vec3 wincoord = glm::vec3(x, window_height - y - 1, depth);
  //glm::vec3 objcoord = glm::unProject(wincoord, view, projection, viewport);

  //printf("Coordinates in object space: %f, %f, %f\n",
    //    objcoord.x, objcoord.y, objcoord.z);

  printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
         x, y, color[0], color[1], color[2], color[3], depth, index);
}

#endif
