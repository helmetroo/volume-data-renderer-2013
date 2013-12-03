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
  
  /*
  for(int vert = 0; vert < (NUM_FACES << 2); vert += 4)
    {
      int face_index = vert >> 2;
      renderPlane(vert, face_index);
    }
  */
  float x = 1.0f;
  float y = 1.0f;
  float z = 1.0f;

  glBegin(GL_QUADS);
	/* Back side */
	glNormal3f(0.0, 0.0, -1.0);
	doVertex(0.0, 0.0, 0.0);
	doVertex(0.0, y, 0.0);
	doVertex(x, y, 0.0);
	doVertex(x, 0.0, 0.0);

	/* Front side */
	glNormal3f(0.0, 0.0, 1.0);
	doVertex(0.0, 0.0, z);
	doVertex(x, 0.0, z);
	doVertex(x, y, z);
	doVertex(0.0, y, z);

	/* Top side */
	glNormal3f(0.0, 1.0, 0.0);
	doVertex(0.0, y, 0.0);
	doVertex(0.0, y, z);
    doVertex(x, y, z);
	doVertex(x, y, 0.0);

	/* Bottom side */
	glNormal3f(0.0, -1.0, 0.0);
	doVertex(0.0, 0.0, 0.0);
	doVertex(x, 0.0, 0.0);
	doVertex(x, 0.0, z);
	doVertex(0.0, 0.0, z);

	/* Left side */
	glNormal3f(-1.0, 0.0, 0.0);
	doVertex(0.0, 0.0, 0.0);
	doVertex(0.0, 0.0, z);
	doVertex(0.0, y, z);
	doVertex(0.0, y, 0.0);

	/* Right side */
	glNormal3f(1.0, 0.0, 0.0);
	doVertex(x, 0.0, 0.0);
	doVertex(x, y, 0.0);
	doVertex(x, y, z);
	doVertex(x, 0.0, z);
	glEnd();

  glPopMatrix(); MatrixStack::popMatrix();
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

      glColor3f(vertices[current_index*3], 
		vertices[(current_index*3)+1], 
		vertices[(current_index*3)+2]);

      glMultiTexCoord3f(GL_TEXTURE6, vertices[current_index*3], 
			vertices[(current_index*3)+1], 
			vertices[(current_index*3)+2]);

      glVertex3f(vertices[current_index*3], 
		 vertices[(current_index*3)+1], 
		 vertices[(current_index*3)+2]);
    }
  glEnd();
}

