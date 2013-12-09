#include "boundingbox.hpp"

BoundingBox::BoundingBox()
{
  rotation_matrix = new float[16]
    {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    };

  indices = new GLuint[NUM_FACES * 4]
    {
      0, 3, 4, 2,  // LEFT
      1, 6, 7, 5,  // RIGHT
      2, 4, 7, 6,  // TOP
      0, 1, 5, 3,  // BOTTOM
      0, 2, 6, 1,  // BACK
      3, 5, 7, 4   // FRONT
    };

  // Unitcube vertices
  vertices = new GLfloat[NUM_FACES * 4] 
    {
      0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f, // 2
      0.0f, 1.0f, 1.0f, // 1
      1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 0.0f, // 3
      1.0f, 1.0f, 1.0f // 4
    };
}

BoundingBox::~BoundingBox()
{
  delete[] vertices;
  delete[] indices;
}

void BoundingBox::draw(void)
{
  glPushMatrix();
  glMultMatrixf(rotation_matrix);

  for(int vert = 0; vert < (NUM_FACES << 2); vert += 4)
    {
      int face_index = vert >> 2;
      renderPlane(vert, face_index);
    }

  glPopMatrix();
}

void BoundingBox::doVertex(GLfloat x, GLfloat y, GLfloat z)
{
  glColor3f(x, y, z);
  glMultiTexCoord3f(GL_TEXTURE6, x, y, z);
  glVertex3f(x, y, z);
}

void BoundingBox::renderPlane(int vert, int face_index)
{
  glBegin(GL_QUADS);
  for(int idx = 0; idx < 4; ++idx)
    {
      int current_index = indices[vert + idx];

      doVertex(vertices[current_index*3], 
		vertices[(current_index*3)+1], 
		vertices[(current_index*3)+2]);
    }
  glEnd();
}

