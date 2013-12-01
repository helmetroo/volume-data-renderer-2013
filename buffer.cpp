#include "buffer.hpp"

Buffer::Buffer(BufferType which, GLuint _width, GLuint _height)
{
  which_to_use = which;
  width = _width;
  height = _height;

  texture_ptr = 0;
  buffer_ptr = 0;

  ++texture_unit;
}

Buffer::~Buffer()
{
}

void Buffer::prepBufferTexture(void)
{
  // Create a texture for the buffer first
  glGenTextures(1, &texture_ptr);
  glBindTexture(GL_TEXTURE_2D, texture_ptr);  
}

void Buffer::setFilter(Buffer::BufferFiltering filter)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}

void Buffer::setWrapping(void)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

// Depth buffer only
void Buffer::setComparison(void)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
}

void Buffer::createBufferTexture(bool with_alpha)
{
  // May need to know how big window is... (want a high res buffer, so
  if(which_to_use == Buffer::BufferType::DEPTH) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  } else {
    // What about alpha?
    if(with_alpha)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    else
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    // So we can use another texture
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void Buffer::prepBuffer(void)
{
  // Create a frame buffer to hold the texture
  glGenFramebuffers(1, &buffer_ptr);
  glBindFramebuffer(GL_FRAMEBUFFER, buffer_ptr);
}

void Buffer::createBuffer(void)
{
  // Attach texture to this new buffer
  if(which_to_use == Buffer::BufferType::DEPTH) {
#ifdef __APPLE__
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_ptr, 0);
#else
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_ptr, 0);
#endif
  } else {
#ifdef __APPLE__
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_ptr, 0);
#else
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_ptr, 0);
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

void Buffer::bindBufferTexture(const char* name)
{
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, texture_ptr);
  glUniform1i(ShaderSystem::getUniformFromCurrentShader(name), 0);
}

void Buffer::unbindBufferTexture(void)
{
  glBindTexture(GL_TEXTURE_2D, 0);
}
