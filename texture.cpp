#include "texture.hpp"

GLint Texture::textureUnit = 0;

Texture::Texture()
{
  ++textureUnit;

  setTranslation(0.0f, 0.0f, 0.0f);
  setRotation(0.0f, 0.0f, 0.0f, 1.0f);
  setScale(1.0f, 1.0f, 1.0f);
}

void Texture::createTexture(bool mipmapped)
{
  enable();
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  if(!mipmapped) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else {
    buildMipmaps();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  }

  passToGpu();

  disable();
  freeImage();
}

void ImageTexture::buildMipmaps(void)
{
  gluBuild2DMipmaps(GL_TEXTURE_2D, image->components, image->width, image->height, image->components, GL_UNSIGNED_BYTE, image->pixels);
}

void ImageTexture::passToGpu(void)
{
  glTexImage2D(GL_TEXTURE_2D, 0, image->components, image->width, image->height, 0, image->format, GL_UNSIGNED_BYTE, image->pixels);
}

void ImageTexture::freeImage(void)
{
  // Allocated C-style, so...
  free(image);
}

void ProceduralTexture::buildMipmaps(void)
{
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, this->image_width, this->image_height, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

void ProceduralTexture::passToGpu(void)
{
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->image_width, this->image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

void ProceduralTexture::freeImage(void)
{
  // Allocated C++ style, so...
  delete[] image;
}

void ImageTexture::readFromFile(const char* file_name)
{
  FILE* image_file = openImageFile(file_name);
  readImageFile(image_file, file_name, &image); 
}

FILE* ImageTexture::openImageFile(const char* file_name)
{
  FILE* file = fopen(file_name, "rb");

  if(file == NULL)
    {
      printf("Oops! Could not open %s\n", file_name);
      exit(1);
    }

  return file;
}

void ImageTexture::readImageFile(FILE* file, const char* file_name, gliGenericImage** dest)
{
  *dest = gliReadTGA(file, const_cast<char*>(file_name));
  fclose(file);
}

void ProceduralTexture::createCheckerboard(int width, int height, int square_size)
{
  this->image_width = width;
  this->image_height = height;

  const int size = width*height*4;
  image = new GLubyte[size];

  GLubyte toggle = 0;
  for(int x = 0; x < size; x += 4)
    {
      int h = x / height;
      GLubyte current_color = ((x & square_size) ^ (h & square_size)) * 255;
      image[x] = image[x+1] = image[x+2] = current_color;
      image[x+3] = 255;
    }
}

void Texture::beginRender(char* texture_name)
{
  enable();
  glBindTexture(GL_TEXTURE_2D, textureID);

  glUniform1i(ShaderSystem::getUniformFromCurrentShader(texture_name), textureUnit);
}

void Texture::beginRenderToSphere(GLUquadric* sphere, char* texture_name)
{
  beginRender(texture_name);
  gluQuadricTexture(sphere, GLU_TRUE);
}

void Texture::setTranslation(float _x, float _y, float _z)
{
  x = _x;
  y = _y;
  z = _z;
}
  
void Texture::setRotation(float _rot_angle, float _rot_axis_x, float _rot_axis_y, float _rot_axis_z)
{
  rot_angle = _rot_angle;
  rot_axis_x = _rot_axis_x;
  rot_axis_y = _rot_axis_y;
  rot_axis_z = _rot_axis_z;
}
  
void Texture::setScale(float _sc_x, float _sc_y, float _sc_z)
{
  sc_x = _sc_x;
  sc_y = _sc_y;
  sc_z = _sc_z;
}

void Texture::transform(void)
{
  glMatrixMode(GL_TEXTURE); MatrixStack::matrixMode(MatrixStack::TEXTURE);
  glLoadIdentity(); MatrixStack::loadIdentity();

  glTranslatef(x, y, z); MatrixStack::translate(x, y, z);
  glRotatef(rot_angle, rot_axis_x, rot_axis_y, rot_axis_z); MatrixStack::rotate(rot_angle, rot_axis_x, rot_axis_y, rot_axis_z);
  glScalef(sc_x, sc_y, sc_z); MatrixStack::scale(sc_x, sc_y, sc_z);

  glMatrixMode(GL_MODELVIEW); MatrixStack::matrixMode(MatrixStack::WORLD);
}
