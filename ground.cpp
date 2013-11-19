#include "ground.hpp"

Ground::Ground()
{
  texture = new ProceduralTexture;
  texture->createCheckerboard(256, 256, 32);
  texture->createTexture(true);
}

Ground::~Ground()
{
  delete texture;
}

void Ground::draw(void)
{
  // Draws the ground plane.
  glPushMatrix();
  glTranslatef(x,y,z);
  glScalef(scale, scale, scale);

  glUniform1i(ShaderSystem::getUniformFromCurrentShader("drawingGround"), 1);
  texture->beginRender("groundCheck");
  texture->transform();
  texture->disable();

  glBegin(GL_QUADS);
  // We drew this in CCW order
  glNormal3f(0,1,0); // 0,1 -> 0,0 -> 1,0 -> 1,1
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1,0,1);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(1,0,1);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(1,0,-1);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1,0,-1);

  glUniform1i(ShaderSystem::getUniformFromCurrentShader("drawingGround"), 0);

  glPopMatrix();
  glEnd();
}
