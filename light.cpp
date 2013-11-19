#include "light.hpp"

// Constants
const GLfloat Light::DEFAULT_AMBIENT[] = {1.0, 1.0, 1.0, 0.25};
const GLfloat Light::DEFAULT_DIFFUSE[] = {1.0, 1.0, 1.0, 1.0};
const GLfloat Light::DEFAULT_SPECULAR[] = {1.0, 1.0, 1.0, 1.0};

Light::Light()
{
  initLightArrays();
  setPos(0.0f, 0.0f, 0.0f, 0.0f);

  view_matrix = Matrix4(1.0f);
  projection_matrix = Matrix4(1.0f);
}

Light::Light(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  initLightArrays();
  setPos(x, y, z, w);

  view_matrix = Matrix4(1.0f);
  projection_matrix = Matrix4(1.0f);
}

Light::~Light()
{
}

void Light::initLightArrays(void)
{
  light_position = new GLfloat[4];
  light_xz_pos = new GLfloat[2];
  light_modelview_matrix = new GLfloat[16];
}

