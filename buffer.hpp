#ifndef BUFFER_H
#define BUFFER_H

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#include <GLUI/glui.h>
#else
#include "GL/glui.h"
#include "GL/glut.h"
#endif

#include "shader.hpp"

class Buffer
{
public:
  enum BufferType
    {
      FRAME,
      DEPTH
    };

  enum BufferFiltering
    {
      NEAREST = GL_NEAREST, 
      LINEAR  = GL_LINEAR
    };

  Buffer(BufferType which, GLuint _width, GLuint _height);
  ~Buffer();

  void prepBufferTexture(void);
  void setFilter(BufferFiltering filter);
  void setWrapping(void);
  void setComparison(void);
  void createBufferTexture(bool with_alpha);
  void createBuffer(void);

  void bind(void);
  void unbind(void);

  void bindBufferTexture(const char* name);
  void unbindBufferTexture(void);

  void clearDepth(void);
  void disableDraw(void);
  void enableDraw(void);

private:
  BufferType which_to_use;
  int width;
  int height;

  static int texture_unit = -1;

  GLuint texture_ptr;
  GLuint buffer_ptr;
};

#endif
