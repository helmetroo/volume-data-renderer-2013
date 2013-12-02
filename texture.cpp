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
  createOnGpu();
  
  setWrapping();

  if(!mipmapped) {
    setFilter();
  } else {
    buildMipmaps();
    setFilterWithMipmap();
  }

  passToGpu();

  disable();
  freeImage();
}


void Texture::beginRender(char* texture_name)
{
  enable();
  bind();

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

void ImageTexture::bind(void)
{
  glBindTexture(GL_TEXTURE_2D, textureID);
}

void ImageTexture::createOnGpu(void)
{
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
}

void ImageTexture::setWrapping(void)
{
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void ImageTexture::setFilter(void)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void ImageTexture::setFilterWithMipmap(void)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
  free(image);
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


/* Volume texture methods */
void VolumeTexture::bind(void)
{
  glBindTexture(GL_TEXTURE_2D, textureID);
}

void VolumeTexture::createOnGpu(void)
{
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_3D, textureID);
}

void VolumeTexture::setWrapping(void)
{
  glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
}

void VolumeTexture::setFilter(void)
{
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void VolumeTexture::setFilterWithMipmap(void)
{
}

void VolumeTexture::buildMipmaps(void)
{

}

void VolumeTexture::passToGpu(void)
{
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, image_width, image_height, image_depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

void VolumeTexture::freeImage(void)
{
  delete[] image;
}
