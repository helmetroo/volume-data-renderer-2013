#ifndef SCENE_H
#define SCENE_H

// Objects
#include "boundingbox.hpp"
#include "fullquad.hpp"

#include "shader.hpp"
#include "trackball.hpp"

#include "buffer.hpp"
#include "texture.hpp"

class Scene
{
public:
  Scene(GLuint view_width, GLuint view_height);
  ~Scene();

  void initObjects(void);

  // Raycasting steps
  void renderBoundingBox(void);
  void raycast(void);
  void outputFinalImage(void);

  void changeVolumeFromFileName(const char* name);

  inline GLuint getWidth(void) { return width; } 
  inline GLuint getHeight(void) { return height; }

  inline BoundingBox* getBoundingBox(void)  { return bounding_box; }
  inline TrackballCamera* getCamera(void)   { return camera; }

private:
  // Objects
  BoundingBox* bounding_box;
  
  // Camera
  TrackballCamera* camera;

  // Volume
  VolumeTexture* volume_texture;

  // Buffer
  OutputBuffer* render_buffer;
  BufferTexture* backface_texture;
  BufferTexture* output_texture;
  GLuint width, height;

  // Full-screen quad
  FullQuad* full_quad;
};

#endif
