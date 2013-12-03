#include "buffer.hpp"

Buffer::Buffer(BufferType which, GLuint _width, GLuint _height)
{
  which_to_use = which;
  width = _width;
  height = _height;

  buffer_ptr = 0;
}

Buffer::~Buffer()
{
}

// Depth buffer only
void Buffer::setComparison(void)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

void Buffer::prepBuffer(void)
{
  // Create a frame buffer to hold the texture
  glGenFramebuffers(1, &buffer_ptr);
  glBindFramebuffer(GL_FRAMEBUFFER, buffer_ptr);
}

void Buffer::attachBufferToTexture(Texture* texture)
{
  // Attach texture to this new buffer.
  // Apple machines have this method as an extension.
  if(which_to_use == Buffer::BufferType::DEPTH) {
#ifdef __APPLE__
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->textureID(), 0);
#else
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->textureID(), 0);
#endif
  } else {
#ifdef __APPLE__
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->textureID(), 0);
#else
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->textureID(), 0);
#endif
  }
}

void Buffer::disableDraw(void)
{
  glDrawBuffer(GL_NONE);

  if(which_to_use == Buffer::BufferType::DEPTH)
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}

void Buffer::enableDraw(void)
{
  if(which_to_use == Buffer::BufferType::DEPTH)
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}
 
void Buffer::bind(void)
{
  // Bind the frame buffer and establish viewport for it
  glBindFramebuffer(GL_FRAMEBUFFER, buffer_ptr);
  
  int tx, ty = 0;
  GLUI_Master.get_viewport_area(&tx, &ty, &width, &height);
  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // No color updates
  if(which_to_use == Buffer::BufferType::DEPTH)
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
}

void Buffer::unbind(void)
{
  int tx, ty = 0;
  GLUI_Master.get_viewport_area(&tx, &ty, &width, &height);
  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
 
void Buffer::clearDepth(void)
{
  glClearDepth(1.0f);
  glClear(GL_DEPTH_BUFFER_BIT);
}
