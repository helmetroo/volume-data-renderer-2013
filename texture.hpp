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

#include <string.h>
#include <math.h>

#include "tga.hpp"
#include "shader.hpp"

#include "codebase.hpp"
#include "ddsbase.hpp"

class Texture
{
public:
  Texture();
  virtual ~Texture() {}

  void createTexture(bool mipmapped);

  inline void enable(void) {
    enableCapability();
    glActiveTexture(GL_TEXTURE0 + texture_unit);
  }

  inline void disable(void) {
    disableCapability();
    glActiveTexture(GL_TEXTURE0);
  }

  inline void coordinate(GLfloat s, GLfloat t) {
    //printf("using unit %d\n", texture_unit);
    glMultiTexCoord2f(GL_TEXTURE0 + texture_unit, s, t);
  };

  void beginRender(const char* texture_name);

  void setTranslation(float _x, float _y, float _z);
  void setRotation(float _rot_angle, float _rot_axis_x, float _rot_axis_y, float _rot_axis_z);
  void setScale(float _sc_x, float _sc_y, float _sc_z);
  void transform(void);

  inline GLuint textureID(void) { return texture_id; }
  inline static GLuint textureUnit(void) { return texture_unit; }

  virtual void buildMipmaps(void) = 0;
  virtual void passToGpu(void) = 0;
  virtual void freeImage(void) = 0;
   
  virtual void createOnGpu(void) = 0;
  virtual void setWrapping(void) = 0;
  virtual void setFilter(void) = 0;
  virtual void setFilterWithMipmap(void) = 0;

  virtual void bind(void) = 0;

  virtual void enableCapability(void) = 0;
  virtual void disableCapability(void) = 0;

protected:
  GLuint texture_id;
  static GLuint texture_unit;

  // Transform
  float rot_angle, rot_axis_x, rot_axis_y, rot_axis_z;
  float sc_x, sc_y, sc_z;
  float x, y, z;
};

/* Texture for use with frame buffer */
class BufferTexture : public Texture
{
public:
  BufferTexture(GLuint _width, GLuint _height) : Texture() 
  { 
    width = _width;
    height = _height; 
  } 

  virtual void enableCapability(void);
  virtual void disableCapability(void);

  virtual void buildMipmaps(void);
  virtual void passToGpu(void);
  virtual void freeImage(void);

  virtual void createOnGpu(void);
  virtual void setWrapping(void);
  virtual void setFilter(void);
  virtual void setFilterWithMipmap(void);

  virtual void bind(void);

private:
  GLuint width, height;
};

/* Represents a texture with depth info */
class VolumeTexture : public Texture
{
public:
  struct Vector3
  {
    GLfloat x, y, z;
    
    Vector3(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z) {}
    GLfloat length(void) { return sqrt(x*x + y*y + z*z); }
      Vector3 operator-(const Vector3& rhs) 
      {
	Vector3 result(x - rhs.x, y - rhs.y, z - rhs.z);
	return result;
      }
      
      Vector3 operator+(const Vector3& rhs) 
      {
	Vector3 result(x + rhs.x, y + rhs.y, z + rhs.z);
	return result;
      }
  };
  VolumeTexture();
  virtual void enableCapability(void);
  virtual void disableCapability(void);

  bool readFromFile(const char* file_name);
  void createTestTexture(void);

  virtual void buildMipmaps(void);
  virtual void passToGpu(void);
  virtual void freeImage(void);

  virtual void createOnGpu(void);
  virtual void setWrapping(void);
  virtual void setFilter(void);
  virtual void setFilterWithMipmap(void);

  virtual void bind(void);

protected:
  unsigned int image_width, image_height, image_depth;
  unsigned int image_component;
  GLubyte* image;

  bool readVolumeData(const char* file_name);
};

#endif
