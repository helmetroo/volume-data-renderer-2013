#include "boundingbox.hpp"

BoundingBox::BoundingBox()
{
  indices = new GLuint[NUM_FACES * 4]
    {
      3, 2, 6, 7, // LEFT
      4, 5, 1, 0, // RIGHT
      0, 3, 7, 4, // TOP
      1, 2, 6, 5,  // BOTTOM
      7, 6, 5, 4, // FRONT
      0, 1, 2, 3 // BACK
    };

  // Unitcube vertices
  vertices = new GLfloat[NUM_FACES * 4] 
    {
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, // 2
      0.0f, 1.0f, 1.0f, // 1
      1.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 1.0f, // 3
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
  glPushMatrix(); MatrixStack::pushMatrix();
  
  for(int vert = 0; vert < (NUM_FACES << 2); vert += 4)
    {
      int face_index = vert >> 2;
      renderPlane(vert, face_index);
    }

  glPopMatrix(); MatrixStack::popMatrix();
}

void BoundingBox::renderPlane(int vert, int face_index)
{
  glBegin(GL_QUADS);
  for(int idx = 0; idx < 4; ++idx)
    {
      int current_index = indices[vert + idx];

      glColor3f(vertices[current_index*3], 
		vertices[(current_index*3)+1], 
		vertices[(current_index*3)+2]);

      glVertex3f(vertices[current_index*3], 
		 vertices[(current_index*3)+1], 
		 vertices[(current_index*3)+2]);

      glMultiTexCoord3f(GL_TEXTURE6, vertices[current_index*3], 
			vertices[(current_index*3)+1], 
			vertices[(current_index*3)+2]);
	  
      glNormal3f(normals[face_index*3], 
		 normals[(face_index*3)+1], 
		 normals[(face_index*3)+2]);
    }
  glEnd();
}

