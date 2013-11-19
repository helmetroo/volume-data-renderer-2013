#include "room.hpp"

Room::Room()
{
  vertices = new GLfloat[NUM_FACES * 4] 
    {
      -1.0,  1.0,  1.0,  // 4
      -1.0, -1.0,  1.0, // 3
      1.0, -1.0,  1.0,
      1.0,  1.0,  1.0,
      -1.0,  1.0, -1.0, // 1
      -1.0, -1.0, -1.0, // 2
      1.0, -1.0, -1.0,
      1.0,  1.0, -1.0
    };

  normals = new GLfloat[NUM_FACES * 3]
    {
      -1.0f, 0.0f, 0.0f, // RIGHT
      1.0f, 0.0f, 0.0f, // LEFT
      0.0f, -1.0f, 0.0f, // BOTTOM
      0.0, -1.0f, 0.0f, // TOP
      0.0f, 0.0f, -1.0f, // FRONT
      0.0f, 0.0f,  1.0f // BACK
    };

  tangents = new GLfloat[NUM_FACES * 3]
    {
      0.0f, 1.0f, 0.0f, // RIGHT
      0.0f, -1.0f, 0.0f, // LEFT
      1.0f, 0.0f, 0.0f, // BOTTOM
      1.0f, 0.0f, 0.0f, // TOP
      1.0f, 0.0f, 0.0f, // FRONT
      1.0f, 0.0f, 0.0f // BACK
    };

  texcoords = new GLfloat[8]
    {
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f
    };

  indices = new GLuint[NUM_FACES * 4]
    {
      3, 2, 6, 7, // RIGHT
      4, 5, 1, 0, // LEFT
      0, 3, 7, 4, // TOP
      1, 2, 6, 5,  // BOTTOM
      7, 6, 5, 4, // FRONT
      0, 1, 2, 3 // BACK
    };

  ImageTexture* floor_normal = new ImageTexture();
  floor_normal->readFromFile("floor_normal_map.tga");
  floor_normal->createTexture(false);
  floor_texture = new BumpMapTexture(floor_normal);

  ImageTexture* brick_normal = new ImageTexture();
  brick_normal->readFromFile("BlueGreenBrick-NormalMap.tga");
  brick_normal->createTexture(false);

  ImageTexture* brick_color = new ImageTexture();
  brick_color->readFromFile("BlueGreenBrick-ColorMap.tga");
  brick_color->createTexture(false);

  brick_texture = new BumpMapTexture(brick_normal, brick_color);
}

Room::~Room()
{
  delete[] vertices;
  delete[] texcoords;
  delete[] normals;
  delete[] indices;

  delete floor_texture;
  delete brick_texture;
}

void Room::draw(void)
{
  glPushMatrix(); MatrixStack::pushMatrix();
  glTranslatef(0.0f, 45.0f, 0.0f); MatrixStack::translate(0.0f, 45.0f, 0.0f);
  glScalef(50.0f, 50.0f, 50.0f); MatrixStack::scale(50.0f, 50.0f, 50.0f);
  
  for(int vert = 0; vert < (NUM_FACES << 2); vert += 4)
    {
      int face_index = vert >> 2;

      // Begin render texture
      if(face_index != Face::BOTTOM) {
	ShaderSystem::useShader(ShaderSystem::PHONG_BUMP_BRICK);
      } else {
	ShaderSystem::useShader(ShaderSystem::PHONG_BUMP_FLOOR);
	floor_texture->enableNormal();
	floor_texture->beginRenderNormal("floorNormalTexture");
      }

      // Default color
      glMaterialfv(GL_FRONT, GL_AMBIENT, GL_AMBIENT_TURQUOISE);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, GL_DIFFUSE_TURQUOISE);
      glMaterialfv(GL_FRONT, GL_SPECULAR, GL_SPECULAR_TURQUOISE);
      glMaterialfv(GL_FRONT, GL_SHININESS, GL_SHININESS_TURQUOISE);
      glMaterialfv(GL_FRONT, GL_EMISSION, GL_EMISSION_TURQUOISE);

      // Render both images if this is not the floor
      if(face_index != Face::BOTTOM) {
	for(int instance = 0; instance < 2; ++instance) {
	  if(instance) {
	    brick_texture->enableNormal();
	    brick_texture->beginRenderNormal("brickNormalTexture");
	  } else {
	    brick_texture->enableColor();
	    brick_texture->beginRenderColor("brickColorTexture");
	  }

	  renderPlane(vert, face_index);

	  if(instance)
	    brick_texture->disableNormal();
	  else
	    brick_texture->disableColor();
	}
      } else {
	renderPlane(vert, face_index);
      }
      
      if(face_index == Face::BOTTOM)
	floor_texture->disableNormal();
    }

  glPopMatrix(); MatrixStack::popMatrix();
}

void Room::renderPlane(int vert, int face_index)
{
  // Bind tangent vector (could have done as attribute, but had issues)
  glUniform3fv(ShaderSystem::getAttributeFromCurrentShader("tangent"), 1, tangents + (face_index*3));

  glBegin(GL_QUADS);
  for(int idx = 0; idx < 4; ++idx)
    {
      int current_index = indices[vert + idx];
      // Let's just use the same texture unit as the floor, 
      // since the texcoords are the same for all planes.
      floor_texture->coordinate(texcoords[idx*2], texcoords[(idx*2)+1]);
	  
      glVertex3f(vertices[current_index*3], 
		 vertices[(current_index*3)+1], 
		 vertices[(current_index*3)+2]);
	  
      glNormal3f(normals[face_index*3], 
		 normals[(face_index*3)+1], 
		 normals[(face_index*3)+2]);
    }
  glEnd();
}

