#include "buffer.hpp"

OutputBuffer::OutputBuffer(GLuint _width, GLuint _height)
{
  width = _width;
  height = _height;

  frame_buffer_ptr = 0;
  render_buffer_ptr = 0;
}

// Depth buffer only
void OutputBuffer::setDepthComparison(void)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

void OutputBuffer::createFrameBuffer(void)
{
  // Create a frame buffer to hold the texture
  glGenFramebuffers(1, &frame_buffer_ptr);
  bindFrameBuffer();
}

void OutputBuffer::createRenderBuffer(void)
{
  //Create a frame buffer to hold the texture
  glGenRenderbuffers(1, &render_buffer_ptr);
  bindRenderBuffer();
}

void OutputBuffer::declareBufferStorage(void)
{
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
}

void OutputBuffer::attachRenderBufferToFrameBuffer(void)
{
  // Attach a render buffer to an existing frame buffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer_ptr);
}

void OutputBuffer::attachFrameBufferToTexture(Texture* texture)
{
  // Attach texture to this new buffer.
  // Apple machines have this method as an extension
#ifdef __APPLE__
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->textureID(), 0);
#else
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->textureID(), 0);
#endif

    //GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
    //glDrawBuffers(1, buffers);
}

void OutputBuffer::disableDraw(void)
{
  glDrawBuffer(GL_NONE);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}

void OutputBuffer::enableDraw(void)
{
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void OutputBuffer::initViewport(void)
{
  int tx, ty = 0;
  GLUI_Master.get_viewport_area(&tx, &ty, &width, &height);
  glViewport(0, 0, width, height);
} 

void OutputBuffer::bindFrameBuffer(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_ptr);
}

void OutputBuffer::unbindFrameBuffer(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OutputBuffer::bindRenderBuffer(void)
{
  glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_ptr);
}

void OutputBuffer::unbindRenderBuffer(void)
{
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
