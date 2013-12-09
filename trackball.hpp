/**
 * Samuel Sweet
 * CS 5610-1 Assignment 2
 *
 * The trackball-based camera definitions.
 */

#ifndef TRACKBALLCAMERA_H
#define TRACKBALLCAMERA_H

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include "shader.hpp"

#include <math.h>

class TrackballCamera
{
public:
  TrackballCamera(float eye_x, float eye_y, float eye_z, 
		  float lookat_x, float lookat_y, float lookat_z);

  void updateRotation(int x, int y);
  void updateMouseState(int button, int state, int x, int y);

  inline void aim(void) {
    gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2], 0, 1, 0);
  }

  inline float* zRef(void) { return &eye[2]; }

  inline void bindPosition(void) {
    glUniform3fv(ShaderSystem::getUniformFromCurrentShader("camPosition"), 1, eye);
  };
    
private:
  GLfloat eye[3];
  GLfloat lookat[3];

  int cur_button;
  int last_x;
  int last_y;

  void crossproduct(float a[3], float b[3], float res[3]);
  float length(float v[3]);
  void normalize(float v[3]);
};

#endif
