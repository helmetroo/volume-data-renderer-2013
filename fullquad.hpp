/**
 * Samuel Sweet
 * CS 5610-1 Project
 *
 * The main program. Handles user interactions
 * as appropriate.
 */

#ifndef FULLQUAD_H
#define FULLQUAD_H

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

class FullQuad
{
public:
  FullQuad();

  void render(void);
  inline void doVertex(float x, float y) 
  {
    glMultiTexCoord2f(GL_TEXTURE7, x, y);
    glVertex2f(x, y); 
  }
};

#endif
