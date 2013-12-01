#ifndef SCENE_H
#define SCENE_H

// Objects
#include "object.hpp"
#include "boundingbox.hpp"

#include "matrixstack.hpp"
#include "shader.hpp"
#include "trackball.hpp"
#include "light.hpp"

class Scene
{
public:
  Scene(GLuint view_width, GLuint view_height);
  ~Scene();

  void initObjects(void);
  void initShaderVars(void);
  void render(void);

  inline Light* getLight(void)              { return light; }
  inline TrackballCamera* getCamera(void)   { return camera; }
  inline BoundingBox* getBoundingBox(void) { return room; }

private:
  // Objects
  BoundingBox* bounding_box;
  
  // Camera
  TrackballCamera* camera;
  Light* light;

  // Buffer
  Buffer* bounding_box_buffer;
  GLuint width, height;
};

#endif
