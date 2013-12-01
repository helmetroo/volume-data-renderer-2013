/**
 * Samuel Sweet
 * CS 5610-1 Assignment 3
 *
 * Contains the definitions
 * for different kinds of texture classes.
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include <math.h>

#include "tga.hpp"
#include "matrixstack.hpp"
#include "shader.hpp"

class Texture
{
public:
  Texture();
  virtual ~Texture() {}

  void createTexture(bool mipmapped);
  virtual void initTexture(void);

  inline void enable(void) {
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
  }

  inline void disable(void) {
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
  }

  inline void coordinate(GLfloat s, GLfloat t) {
    //printf("using unit %d\n", textureUnit);
    glMultiTexCoord2f(GL_TEXTURE0 + textureUnit, s, t);
  };

  void beginRender(char* texture_name);
  void beginRenderToSphere(GLUquadric* sphere, char* texture_name);

  void setTranslation(float _x, float _y, float _z);
  void setRotation(float _rot_angle, float _rot_axis_x, float _rot_axis_y, float _rot_axis_z);
  void setScale(float _sc_x, float _sc_y, float _sc_z);
  void transform(void);

protected:
  virtual void buildMipmaps(void) = 0;
  virtual void passToGpu(void) = 0;
  virtual void freeImage(void) = 0;

  GLuint textureID;
  static GLint textureUnit;

  float rot_angle, rot_axis_x, rot_axis_y, rot_axis_z;
  float sc_x, sc_y, sc_z;
  float x, y, z;
};

/* Represents a texture generated from an image file (TGA) */
class ImageTexture : public Texture
{
public:
  ImageTexture() : Texture() {}
  void readFromFile(const char* file_name);

protected:
  virtual void buildMipmaps(void);
  virtual void passToGpu(void);
  virtual void freeImage(void);

  FILE* openImageFile(const char* name);
  void readImageFile(FILE* file, const char* name, gliGenericImage** dest);

  gliGenericImage* image;
};

/* Represents a normal texture and respective */
class ProceduralTexture : public Texture
{
public:
  ProceduralTexture() : Texture() {}
  void createCheckerboard(int width, int height, int square_size);

private:
  virtual void buildMipmaps(void);
  virtual void passToGpu(void);
  virtual void freeImage(void);

  GLubyte* image;
  int image_width, image_height;
};

class VolumeTexture : public Texture
{
public:
  VolumeTexture() : Texture() {}

private:
  virtual void buildMipmaps(void);
  virtual void passToGpu(void);
  virtual void freeImage(void);
  int image_width, image_height, image_depth;
};

#endif