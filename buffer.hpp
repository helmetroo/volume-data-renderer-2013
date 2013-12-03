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
#include "texture.hpp"

class Buffer
{
public:
  enum BufferType
    {
      FRAME,
      DEPTH,
      RENDER
    };

  Buffer(BufferType which, GLuint _width, GLuint _height);
  ~Buffer();

  void prepBuffer(void);
  void setComparison(void);
  void attachBufferToTexture(Texture* texture);

  void bind(void);
  void unbind(void);

  void clearDepth(void);
  void disableDraw(void);
  void enableDraw(void);

private:
  BufferType which_to_use;
  int width;
  int height;

  GLuint buffer_ptr;
};

#endif
