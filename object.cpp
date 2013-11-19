#include "object.hpp"

// Teapot implementations
Teapot::Teapot()
{
  pos_x = init_x = 0.0f;
  pos_y = init_y = 3.0f;
  pos_z = init_z = 0.0f;
}

Teapot::~Teapot()
{
}

void Teapot::draw(void)
{
  glPushMatrix();
  glTranslatef(pos_x, pos_y, pos_z);

  glMaterialfv(GL_FRONT, GL_AMBIENT, GL_AMBIENT_SILVER);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, GL_DIFFUSE_SILVER);
  glMaterialfv(GL_FRONT, GL_SPECULAR, GL_SPECULAR_SILVER);
  glMaterialfv(GL_FRONT, GL_SHININESS, GL_SHININESS_SILVER);
  glMaterialfv(GL_FRONT, GL_EMISSION, GL_EMISSION_SILVER);

  glutSolidTeapot(2.5);
  glPopMatrix();
}

// Cube implementations
Cube::Cube()
{
  pos_x = init_x = 0.0f;
  pos_y = init_y = -2.0f;
  pos_z = init_z = 0.0f;
}

Cube::~Cube()
{
}

void Cube::draw(void)
{
  glPushMatrix();
  glTranslatef(pos_x, pos_y, pos_z);
  
  glMaterialfv(GL_FRONT, GL_AMBIENT, GL_AMBIENT_PEARL);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, GL_DIFFUSE_PEARL);
  glMaterialfv(GL_FRONT, GL_SPECULAR, GL_SPECULAR_PEARL);
  glMaterialfv(GL_FRONT, GL_SHININESS, GL_SHININESS_PEARL);
  glMaterialfv(GL_FRONT, GL_EMISSION, GL_EMISSION_PEARL);

  glutSolidCube(6.0);
  glPopMatrix();
}
