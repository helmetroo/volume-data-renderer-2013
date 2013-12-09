#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include <math.h>
#include <algorithm>

#include "glm/glm.hpp"

#include "matrixstack.hpp"
#include "shader.hpp"
#include "texture.hpp"

#define NUM_FACES 6
class BoundingBox
{
public:
  enum Face
    {
      LEFT = 0,
      RIGHT = 1,
      TOP = 2,
      BOTTOM = 3,
      FRONT = 4,
      BACK = 5
    };

  BoundingBox();
  ~BoundingBox();

  void draw(void);

private:
  GLfloat* vertices;
  GLuint* indices;

  void doVertex(GLfloat x, GLfloat y, GLfloat z);
  void renderPlane(int vert, int face_index);
};

#endif
