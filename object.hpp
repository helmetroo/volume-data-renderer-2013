/*
 * Samuel Sweet
 * CS 5610-1 Assignment 3
 *
 * A class to represent drawable objects.
 */

#ifndef OBJECT_H
#define OBJECT_H

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include "matrixstack.hpp"

// Materials for use with objects
const float GL_AMBIENT_PEARL[] = {0.25, 0.20725, 0.20725, 0.922};
const float GL_DIFFUSE_PEARL[] = {1.0, 0.829, 0.829, 0.922};
const float GL_SPECULAR_PEARL[] = {0.296648, 0.296648, 0.296648, 0.922};
const float GL_SHININESS_PEARL[] = { 11.264 };
const float GL_EMISSION_PEARL[] = {0.0f, 0.0f, 0.0f, 0.0f};

const float GL_AMBIENT_SILVER[] = {0.19225f, 0.19225f, 0.19225f, 1.0f};
const float GL_DIFFUSE_SILVER[] = {0.50754f, 0.50754f, 0.50754f, 1.0f};
const float GL_SPECULAR_SILVER[] = {0.508273f, 0.508273f, 0.508273f, 1.0f};
const float GL_SHININESS_SILVER[] = {51.2f};
const float GL_EMISSION_SILVER[] = {0.0f, 0.0f, 0.0f, 0.0f};

// Material definitions
const float GL_AMBIENT_CHROME[] = {0.25, 0.25, 0.25, 1.0};
const float GL_DIFFUSE_CHROME[] = {0.4, 0.4, 0.4, 1.0};
const float GL_SPECULAR_CHROME[] = {0.774597, 0.774597, 0.774597, 1.0};
const float GL_SHININESS_CHROME[] = {76.8f};
const float GL_EMISSION_CHROME[] = {0.0f, 0.0f, 0.0f, 0.0f};

const float GL_AMBIENT_EYE[] = {0.8f, 0.7f, 0.7f, 1.0f};
const float GL_DIFFUSE_EYE[] = {0.8f, 0.8f, 0.8f, 1.0f};
const float GL_SPECULAR_EYE[] = {0.0f, 0.0f, 0.0f, 1.0f};
const float GL_SHININESS_EYE[] = {21.2f};
const float GL_EMISSION_EYE[] = {0.45f, 0.45f, 0.45f, 0.25f};

const float GL_AMBIENT_JADE[] = {0.135, 0.2225, 0.1575, 0.95};
const float GL_DIFFUSE_JADE[] = {0.54, 0.89, 0.63, 0.95};
const float GL_SPECULAR_JADE[] = {0.316228, 0.316228, 0.316228, 0.95};
const float GL_SHININESS_JADE[] = {12.8f};
const float GL_EMISSION_JADE[] = {0.0f, 0.0f, 0.0f, 0.0f};

const float GL_AMBIENT_RUBBER[] = {0.02f, 0.02f, 0.02f, 1.0f};
const float GL_DIFFUSE_RUBBER[] = {0.01f, 0.01f, 0.01f, 1.0f};
const float GL_SPECULAR_RUBBER[] = {0.4f, 0.4f, 0.4f, 1.0f};
const float GL_SHININESS_RUBBER[] = {99.0f};
const float GL_EMISSION_RUBBER[] = {0.05f, 0.05f, 0.05f, 0.05f};

const float GL_AMBIENT_COPPER[] = {0.19125, 0.0735, 0.0225, 1.0};
const float GL_DIFFUSE_COPPER[] = {0.7038, 0.27048, 0.0828, 1.0};
const float GL_SPECULAR_COPPER[] = {0.256777, 0.137622, 0.086014, 1.0};
const float GL_SHININESS_COPPER[] = { 12.8 };
const float GL_EMISSION_COPPER[] = {0.05f, 0.05f, 0.05f, 0.05f};

class Object
{
public:
  virtual ~Object() {}

  virtual void draw(void) = 0;

protected:
  float pos_x, pos_y, pos_z;
  float init_x, init_y, init_z;
};

// Teapot for scene
class Teapot : public Object
{
public:
  Teapot();
  ~Teapot();

  void draw(void);
};

// Cube for scene
class Cube : public Object
{
public:
  Cube();
  ~Cube();

  void draw(void);
};

#endif
