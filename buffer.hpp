/*
 * CS 5610-1 Project
 *
 * Represents a render buffer with final output frame buffer. 
 */

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

class OutputBuffer
{
public:
  OutputBuffer(GLuint _width, GLuint _height);

  void createFrameBuffer(void);
  void createRenderBuffer(void);
  void declareBufferStorage(void);

  void setDepthComparison(void);
  void attachRenderBufferToFrameBuffer(void);
  void attachFrameBufferToTexture(Texture* texture);

  void initViewport(void);

  void bindFrameBuffer(void);
  void unbindFrameBuffer(void);
  void bindRenderBuffer(void);
  void unbindRenderBuffer(void);

  void disableDraw(void);
  void enableDraw(void);

private:
  int width;
  int height;

  GLuint frame_buffer_ptr;
  GLuint render_buffer_ptr;
};

#endif
