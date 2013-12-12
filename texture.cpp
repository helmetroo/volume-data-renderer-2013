#include "texture.hpp"

GLuint Texture::texture_unit = 2;

Texture::Texture()
{
  ++texture_unit;

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

void Texture::beginRender(const char* texture_name)
{
  enable();
  bind();

  glUniform1i(ShaderSystem::getUniformFromCurrentShader(texture_name), texture_unit);
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
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();

  glTranslatef(x, y, z);
  glRotatef(rot_angle, rot_axis_x, rot_axis_y, rot_axis_z);
  glScalef(sc_x, sc_y, sc_z);

  glMatrixMode(GL_MODELVIEW);
}

void BufferTexture::enableCapability(void)
{
  glEnable(GL_TEXTURE_2D);
}

void BufferTexture::disableCapability(void)
{
  glDisable(GL_TEXTURE_2D);
}

void BufferTexture::bind(void)
{
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

void BufferTexture::createOnGpu(void)
{
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void BufferTexture::setWrapping(void)
{
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

void BufferTexture::setFilter(void)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void BufferTexture::setFilterWithMipmap(void)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void BufferTexture::buildMipmaps(void)
{
  
}

void BufferTexture::passToGpu(void)
{
#ifdef __APPLE__
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
#else
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
#endif
}

void BufferTexture::freeImage(void)
{
}

VolumeTexture::VolumeTexture() : Texture() 
{
  image_width = 0;
  image_height = 0;
  image_depth = 0;

  image_component = GL_RGBA;

  image = NULL;
}

void VolumeTexture::enableCapability(void)
{
  glEnable(GL_TEXTURE_3D);
}

void VolumeTexture::disableCapability(void)
{
  glDisable(GL_TEXTURE_3D);
}

/* Volume texture methods */
void VolumeTexture::bind(void)
{
  glBindTexture(GL_TEXTURE_3D, texture_id);
}

void VolumeTexture::createOnGpu(void)
{
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_3D, texture_id);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void VolumeTexture::setWrapping(void)
{
  glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
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
  // Determine the number of components per color, and have OGL
  // interpret the texture based on said components.
  if(image_component == 1)
    glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, image_width, image_height, image_depth, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image);
  else if(image_component == 3)
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, image_width, image_height, image_depth, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  else if(image_component == 4)
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, image_width, image_height, image_depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

bool VolumeTexture::readFromFile(const char* file_name)
{
  char* extension = strrchr(const_cast<char*>(file_name), '.');
  int is_pvm = !(strcmp(extension, ".pvm"));

  if(!is_pvm) {
    printf("Cannot read file. Please check to make sure it has the extension '.pvm' at the end.\n");
    return false;
  }
 
  return readVolumeData(file_name);
}

bool VolumeTexture::readVolumeData(const char* file_name)
{
  unsigned char* new_volume;
  unsigned int width, height, depth, components;
  float scale_x, scale_y, scale_z;

  new_volume = readPVMvolume(const_cast<char*>(file_name), &width, &height, &depth, &components, &scale_x, &scale_y, &scale_z);
  if(!new_volume)
    {
      printf("OOPS! %s could not be found or read.\n", file_name);
      return false;
    } 

  // Set volume.
  printf("Dataset %s read SUCCESSFULLY!\n", file_name);
  printf("Image dimensions: %d x %d x %d\n", width, height, depth);
  printf("Image components: %d\n", components);
  image_width = width;
  image_height = height;
  image_depth = depth;
  image_component = components;
  image = new_volume;

  return true;
}

void VolumeTexture::freeImage(void)
{
  delete[] image;
}
