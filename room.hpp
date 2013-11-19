#ifndef ROOM_H
#define ROOM_H

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
#include "object.hpp"
#include "shader.hpp"
#include "texture.hpp"

typedef glm::mat4 Matrix4;
typedef glm::vec4 Vector4;

// Material definition
const float GL_AMBIENT_TURQUOISE[] = {0.1f, 0.18725f, 0.1745f, 1.0f};
const float GL_DIFFUSE_TURQUOISE[] = {0.396f, 0.74151f, 0.69102f, 1.0f};
const float GL_SPECULAR_TURQUOISE[] = {0.297254f, 0.30829f, 0.306678f, 1.0f};
const float GL_SHININESS_TURQUOISE[] = {12.8f};
const float GL_EMISSION_TURQUOISE[] = {0.05f, 0.05f, 0.05f, 0.05f};

#define NUM_FACES 6
class Room : public Object
{
public:
  enum Face
    {
      RIGHT = 0,
      LEFT = 1,
      TOP = 2,
      BOTTOM = 3,
      FRONT = 4,
      BACK = 5
    };

  Room();
  ~Room();

  void defineMapFromFiles(const char* const* texture_names);
  void draw(void);

private:
  GLuint textureObj[NUM_FACES]; 
  GLfloat* vertices;
  GLuint* indices;
  GLfloat* texcoords;
  GLfloat* normals;
  GLfloat* tangents;

  BumpMapTexture* floor_texture;
  BumpMapTexture* brick_texture;

  void renderPlane(int vert, int face_index);
};

#endif
