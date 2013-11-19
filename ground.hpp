#ifndef GROUND_H
#define GROUND_H

#include <math.h>
#include "texture.hpp"

class Ground
{
public:
  Ground();
  ~Ground();

  void draw(void);

private:
  ProceduralTexture* texture;

  const float scale = 50.0f;
  const float x = 0.0f;
  const float y = -1.0f;
  const float z = 0.0f;
};

#endif
