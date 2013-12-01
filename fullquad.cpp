#include "fullquad.hpp"

FullQuad::FullQuad()
{
}

void FullQuad::render(void)
{
  glDisable(GL_DEPTH_TEST);

  glBegin(GL_TRIANGLE_STRIP);
  doVertex(0.0f, 0.0f);
  doVertex(1.0f, 0.0f);
  doVertex(1.0f, 1.0f);
  doVertex(0.0f, 1.0f);
  glEnd();

  glEnable(GL_DEPTH_TEST);
}
