#ifndef SCENE_H
#define SCENE_H

// Objects
#include "object.hpp"
#include "dog.hpp"
#include "room.hpp"

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

  inline Dog* getDog(void)                { return dog; }
  inline Light* getLight(void)            { return light; }
  inline TrackballCamera* getCamera(void) { return camera; }
  inline Room* getRoom(void)              { return room; }

  inline int* isUsingToonShading() { return &in_toon_mode; }

  inline static void onChangeDogShading(int ignore) {
    
  }

private:
  // Objects
  Dog* dog;
  Room* room;
  
  // Camera
  TrackballCamera* camera;
  Light* light;

  // Buffer
  GLuint width, height;

  // State
  int in_toon_mode;
};

#endif
