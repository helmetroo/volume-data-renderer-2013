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
  glMatrixMode(GL_TEXTURE); MatrixStack::matrixMode(MatrixStack::TEXTURE);
  glLoadIdentity(); MatrixStack::loadIdentity();

  glTranslatef(x, y, z); MatrixStack::translate(x, y, z);
  glRotatef(rot_angle, rot_axis_x, rot_axis_y, rot_axis_z); MatrixStack::rotate(rot_angle, rot_axis_x, rot_axis_y, rot_axis_z);
  glScalef(sc_x, sc_y, sc_z); MatrixStack::scale(sc_x, sc_y, sc_z);

  glMatrixMode(GL_MODELVIEW); MatrixStack::matrixMode(MatrixStack::WORLD);
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

void VolumeTexture::readFromFile(const char* file_name)
{
  char* extension = strrchr(file_name, '.');
  int is_pvm = !(strcmp(extension, "pvm"));

  if(!is_pvm) {
    printf("Cannot read file. Must be PVM file.\n");
    exit(1);
  }
 
  image = readVolumeData(file_name);
}

void VolumeTexture::createTestTexture(void)
{
  int VOLUME_TEX_SIZE = 128;
  int size = VOLUME_TEX_SIZE*VOLUME_TEX_SIZE*VOLUME_TEX_SIZE* 4;
	GLubyte *data = new GLubyte[size];

	for(int x = 0; x < VOLUME_TEX_SIZE; x++)
	{for(int y = 0; y < VOLUME_TEX_SIZE; y++)
	{for(int z = 0; z < VOLUME_TEX_SIZE; z++)
	{
		data[(x*4)   + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = z%250;
		data[(x*4)+1 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = y%250;
		data[(x*4)+2 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 250;
		data[(x*4)+3 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 230;
	  	
		Vector3 p =	Vector3(x,y,z)- Vector3(VOLUME_TEX_SIZE-20,VOLUME_TEX_SIZE-30,VOLUME_TEX_SIZE-30);
		bool test = (p.length() < 42);
		if(test)
			data[(x*4)+3 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 0;

		p =	Vector3(x,y,z)- Vector3(VOLUME_TEX_SIZE/2,VOLUME_TEX_SIZE/2,VOLUME_TEX_SIZE/2);
		test = (p.length() < 24);
		if(test)
			data[(x*4)+3 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 0;

		
		if(x > 20 && x < 40 && y > 0 && y < VOLUME_TEX_SIZE && z > 10 &&  z < 50)
		{
			
			data[(x*4)   + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 100;
		    data[(x*4)+1 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 250;
		    data[(x*4)+2 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = y%100;
			data[(x*4)+3 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 250;
		}

		if(x > 50 && x < 70 && y > 0 && y < VOLUME_TEX_SIZE && z > 10 &&  z < 50)
		{
			
			data[(x*4)   + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 250;
		    data[(x*4)+1 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 250;
		    data[(x*4)+2 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = y%100;
			data[(x*4)+3 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 250;
		}

		if(x > 80 && x < 100 && y > 0 && y < VOLUME_TEX_SIZE && z > 10 &&  z < 50)
		{
			
			data[(x*4)   + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 250;
		    data[(x*4)+1 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 70;
		    data[(x*4)+2 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = y%100;
			data[(x*4)+3 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 250;
		}

		p =	Vector3(x,y,z)- Vector3(24,24,24);
		test = (p.length() < 40);
		if(test)
			data[(x*4)+3 + (y * VOLUME_TEX_SIZE * 4) + (z * VOLUME_TEX_SIZE * VOLUME_TEX_SIZE * 4)] = 0;

			
	}}}



	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	  glGenTextures(1, &texture_id);
	  glBindTexture(GL_TEXTURE_3D, texture_id);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexImage3D(GL_TEXTURE_3D, 0,GL_RGBA, VOLUME_TEX_SIZE, VOLUME_TEX_SIZE,VOLUME_TEX_SIZE,0, GL_RGBA, GL_UNSIGNED_BYTE,data);

	delete []data;
}

GLubyte* VolumeTexture::readVolumeData(const char* file_name)
{
  unsigned char* volume;
  unsigned int width, height, depth, components;
  float scale_x, scale_y, scale_z;

  volume = readPVMvolume(const_cast<char*>(file_name), &width, &height, &depth, &components, &scale_x, &scale_y, &scale_z);

  if(!volume)
    {
      printf("OOPS! %s could not be found or read.\n", file_name);
      return NULL;
    } 
  else 
    {
      return static_cast<GLubyte*>(volume);
    }
}

void VolumeTexture::freeImage(void)
{
  //delete[] image;
}
