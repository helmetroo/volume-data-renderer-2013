/**
 * Samuel Sweet
 * CS 5610-1 Assignment 3
 *
 * Contains the implementations
 * for the drawing and animation methods 
 * defined in dog.hpp.
 */

#include "dog.hpp"

const float Dog::RADS = M_PI / 180.0;

Dog::Dog()
{
  dog = new DogModel;

  pos_x = init_x = -0.25f;
  pos_y = init_y = 0.15f;
  pos_z = init_z = 0.0f;

  dog_speed = 0.02f;
  dog_angle = 0.0f;
  dog_direc_vect = new float[3] { 1.0f, 0.0f, 0.0f };

  BACK_L_LEG_ORIG = new float[3] { -0.15f, 0.0f, -0.05f };
  BACK_R_LEG_ORIG = new float[3] { -0.15f, 0.0f, 0.05f };
  FRONT_L_LEG_ORIG = new float[3] { 0.0f, 0.0f, -0.05f };
  FRONT_R_LEG_ORIG = new float[3] { 0.0f, 0.0f, 0.05f };

  back_l_joint_angle = -5.0f;
  back_r_joint_angle = 5.0f;
  front_l_joint_angle = -5.0f;
  front_r_joint_angle = 5.0f;
  angle_time = 0.0f;

  // body (definitions made from right to left)
  Point::definePoint( &(dog->body_defn[0]), 0.1f, 0.04f, 0.0f, 1.0, 1.1, 1.0, 90.0f );
  Point::definePoint( &(dog->body_defn[1]), 0.06f, 0.04f, 0.0f, 1.0, 1.1, 1.0, 90.0f );
  Point::definePoint( &(dog->body_defn[2]), 0.0f, 0.04f, 0.0f, 1.0, 1.1, 1.0, 90.0f );
  Point::definePoint( &(dog->body_defn[3]), -0.06f, 0.04f, 0.0f, 1.0, 1.25, 1.0, 90.0f );
  Point::definePoint( &(dog->body_defn[4]), -0.145f, 0.055f, 0.0f, 1.0, 0.25, 0.25, -15.0f);

  // head (0), jaw (1), nose(2), ears (3-4)
  Point::definePoint( &(dog->head_defn[0]), 0.2f, 0.065f, 0.0f, 1.5, 1.5, 1.5, 0.0f );
  Point::definePoint( &(dog->head_defn[1]), 0.26f, 0.055f, 0.0f, 0.75, 0.75, 0.85, 0.0f );
  Point::definePoint( &(dog->head_defn[2]), 0.28f, 0.065f, 0.0f, 0.45, 0.45, 0.65, 0.0f );
  Point::definePoint( &(dog->head_defn[3]), 0.19f, 0.12f, -0.05f, 0.25, 0.65, 0.45, 30.0f );
  Point::definePoint( &(dog->head_defn[4]), 0.19f, 0.12f, 0.05f, 0.25, 0.65, 0.45, 30.0f );

  // These leg points are defined RELATIVE from each other. 
  // So (0.1, 0.0, 0.0) would be the starting point of the leg.
  Point::definePoint( &(dog->leg_defn[0]), 0.1f, 0.0f, 0.0f, 0.35f, 1.0, 0.15f, 0.0f );
  Point::definePoint( &(dog->leg_defn[1]), 0.01f, -0.075f, 0.0f, 0.25f, 1.0, 0.15f, 0.0f );
  Point::definePoint( &(dog->leg_defn[2]), 0.01f, -0.05f, 0.0f, 0.45f, 0.3f, 0.45f, 0.0f );

  // eyes
  Point::definePoint( &(dog->eyes_defn[0]), 0.25f, 0.1f, 0.03f, 0.35f, 0.35f, 0.4f, 0.0f );
  Point::definePoint( &(dog->eyes_defn[1]), 0.265f, 0.1f, 0.03f, 0.15f, 0.15f, 0.15f, 0.0f );
  Point::definePoint( &(dog->eyes_defn[2]), 0.25f, 0.1f, -0.03f, 0.35f, 0.35f, 0.4f, 0.0f );
  Point::definePoint( &(dog->eyes_defn[3]), 0.265f, 0.1f, -0.03f, 0.15f, 0.15f, 0.15f, 0.0f );

  rotateDirection();
}

Dog::~Dog()
{
  delete dog;
}

void Dog::updateAnim()
{
  // Dog moves relative to the direction pointed by the direction vector
  pos_x += dog_speed * dog_direc_vect[0];
  pos_z += dog_speed * dog_direc_vect[2];

  // Each leg is offset by a different phase shift to imitate natural motion
  back_l_joint_angle = 20.0f*sin(120*dog_speed*angle_time + 2.0f);
  angle_time += 0.005f;

  back_r_joint_angle = 20.0f*sin(120*dog_speed*angle_time + 4.0f);
  angle_time += 0.005f;

  front_l_joint_angle = 20.0f*sin(120*dog_speed*angle_time + 6.0f);
  angle_time += 0.005f;

  front_r_joint_angle = 20.0f*sin(120*dog_speed*angle_time + 8.0f);
  angle_time += 0.005f;

  // Bound the angle time by the sine period
  float period = 2.0*M_PI / dog_speed;
  if(angle_time >= period)
    angle_time = 0.0f;

  // Handle phase shift for tail wag
  glUniform1f(ShaderSystem::getUniformFromCurrentShader("phaseShift"), angle_time);
}

// Alters the direction vector so that the dog
// will properly face the direction it rotates.
void Dog::rotateDirection()
{
  // The dog is confined to the plane (its y position remains constant)
  // Based off the basic 2D rotation formula, with
  // calculations evaluated. Normalizing the vector is superfluous as its length is always 1.
  dog_direc_vect[0] = cos(RADS * dog_angle);
  dog_direc_vect[2] = -sin(RADS * dog_angle);
}

// Sequentially draws all parts of the dog.
void Dog::draw()
{
  glPushMatrix(); MatrixStack::pushMatrix();
   
  // Dog rotates about its center.
  glTranslatef(pos_x, pos_y, pos_z);    MatrixStack::translate(pos_x, pos_y, pos_z);
  glScalef(30.0f, 30.0f, 30.0f);        MatrixStack::scale(30.0f, 30.0f, 30.0f);
  glRotatef(dog_angle, 0.0, 1.0, 0.0);  MatrixStack::rotate(dog_angle, 0.0, 1.0, 0.0);

  glPushMatrix(); MatrixStack::pushMatrix();
  drawBody();
  drawHead();
  drawEyes();
  drawFrontLegs();
  drawBackLegs();
  glPopMatrix();  MatrixStack::popMatrix();

  glPopMatrix();  MatrixStack::popMatrix();
}

void Dog::drawBody()
{
  int idx = 0;

  // For each sphere in the array, translate/scale/rotate it
  // as appropriate. I decided to leave material definitions
  // outside of the dog struct, since the init code would get extremely messy.
  while(idx < NUM_BODY_SPHERES)
    {
      glPushMatrix();  MatrixStack::pushMatrix();
      // Dog rotates about z-axis
      glTranslatef(dog->body_defn[idx].x, dog->body_defn[idx].y, dog->body_defn[idx].z);
      MatrixStack::translate(dog->body_defn[idx].x, dog->body_defn[idx].y, dog->body_defn[idx].z);

      glRotatef(dog->body_defn[idx].rot_angle, 0.0f, 0.0f, 1.0f);
      MatrixStack::rotate(dog->body_defn[idx].rot_angle, 0.0f, 0.0f, 1.0f);
      
      glScalef(dog->body_defn[idx].sc_x, dog->body_defn[idx].sc_y, dog->body_defn[idx].sc_z);
      MatrixStack::scale(dog->body_defn[idx].sc_x, dog->body_defn[idx].sc_y, dog->body_defn[idx].sc_z);

      glMaterialfv(GL_FRONT, GL_AMBIENT, GL_AMBIENT_PEARL);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, GL_DIFFUSE_PEARL);
      glMaterialfv(GL_FRONT, GL_SPECULAR, GL_SPECULAR_PEARL);
      glMaterialfv(GL_FRONT, GL_SHININESS, GL_SHININESS_PEARL);
      glMaterialfv(GL_FRONT, GL_EMISSION, GL_EMISSION_PEARL);

      // The indices for the tail
      if(idx >= 4)
	glUniform1i(ShaderSystem::getUniformFromCurrentShader("isTail"), 1);

      gluSphere(dog->body_defn[idx].sphereMesh, 0.05f, 30, 30);
      glPopMatrix();  MatrixStack::popMatrix();
      ++idx;
    }

  // Finished with the tail, so unset flag
  glUniform1i(ShaderSystem::getUniformFromCurrentShader("isTail"), 0);
}

void Dog::drawFrontLegs()
{
  glPushMatrix();  MatrixStack::pushMatrix();
  glTranslatef(FRONT_R_LEG_ORIG[0], FRONT_R_LEG_ORIG[1], FRONT_R_LEG_ORIG[2]);
  MatrixStack::translate(FRONT_R_LEG_ORIG[0], FRONT_R_LEG_ORIG[1], FRONT_R_LEG_ORIG[2]);
  drawLeg(front_r_joint_angle);
  glPopMatrix();   MatrixStack::popMatrix();


  glPushMatrix();  MatrixStack::pushMatrix();
  glTranslatef(FRONT_L_LEG_ORIG[0], FRONT_L_LEG_ORIG[1], FRONT_L_LEG_ORIG[2]);
  MatrixStack::translate(FRONT_L_LEG_ORIG[0], FRONT_L_LEG_ORIG[1], FRONT_L_LEG_ORIG[2]);
  drawLeg(front_l_joint_angle);
  glPopMatrix();   MatrixStack::popMatrix();
}

void Dog::drawBackLegs()
{
  glPushMatrix();  MatrixStack::pushMatrix();
  glTranslatef(BACK_R_LEG_ORIG[0], BACK_R_LEG_ORIG[1], BACK_R_LEG_ORIG[2]);
  MatrixStack::translate(BACK_R_LEG_ORIG[0], BACK_R_LEG_ORIG[1], BACK_R_LEG_ORIG[2]);
  drawLeg(back_r_joint_angle);
  glPopMatrix();   MatrixStack::popMatrix();


  glPushMatrix();  MatrixStack::pushMatrix();
  glTranslatef(BACK_L_LEG_ORIG[0], BACK_L_LEG_ORIG[1], BACK_L_LEG_ORIG[2]);
  MatrixStack::translate(BACK_L_LEG_ORIG[0], BACK_L_LEG_ORIG[1], BACK_L_LEG_ORIG[2]);
  drawLeg(back_l_joint_angle);
  glPopMatrix();   MatrixStack::popMatrix();
}

void Dog::drawLeg(float leg_angle)
{
  // The offset is used for the lower joint
  int idx = 0;
  float offset = 0.0f;
   
  while(idx < NUM_LEG_SPHERES)
    {
      // No push/pop matrix calls as the spheres for the legs are 
      // defined relative from each other.      
      glTranslatef(dog->leg_defn[idx].x, dog->leg_defn[idx].y, dog->leg_defn[idx].z);
      MatrixStack::translate(dog->leg_defn[idx].x, dog->leg_defn[idx].y, dog->leg_defn[idx].z);

      glRotatef(leg_angle + offset, 0.0f, 0.0f, 1.0f);
      MatrixStack::rotate(leg_angle + offset, 0.0f, 0.0f, 1.0f);

      glScalef(dog->leg_defn[idx].sc_x, dog->leg_defn[idx].sc_y, dog->leg_defn[idx].sc_z);
      MatrixStack::scale(dog->leg_defn[idx].sc_x, dog->leg_defn[idx].sc_y, dog->leg_defn[idx].sc_z);

      glMaterialfv(GL_FRONT, GL_AMBIENT, GL_AMBIENT_JADE);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, GL_DIFFUSE_JADE);
      glMaterialfv(GL_FRONT, GL_SPECULAR, GL_SPECULAR_JADE);
      glMaterialfv(GL_FRONT, GL_SHININESS, GL_SHININESS_JADE);

      gluSphere(dog->leg_defn[idx].sphereMesh, 0.05f, 10, 10);

      // Undo the scale and rotation so we can correctly scale the next part of the leg    
      glScalef(1.0f/dog->leg_defn[idx].sc_x, 1.0f/dog->leg_defn[idx].sc_y, 1.0f/dog->leg_defn[idx].sc_z);
      MatrixStack::scale(1.0f/dog->leg_defn[idx].sc_x, 1.0f/dog->leg_defn[idx].sc_y, 1.0f/dog->leg_defn[idx].sc_z);

      // Give the lower joint a larger rotation angle.
      offset += 10.0f;
      ++idx;
    }
}

// This consists of a physical head, jaw and nose.
void Dog::drawHead()
{
  int idx = 0;
  glPushMatrix(); MatrixStack::pushMatrix();

  const float* curAmbient = NULL;
  const float* curDiffuse = NULL;
  const float* curSpecular = NULL;
  const float* curShininess = NULL;
  const float* curEmission = NULL;

  while(idx < NUM_HEAD_SPHERES)
    {
      glPushMatrix(); MatrixStack::pushMatrix();
      glTranslatef(dog->head_defn[idx].x, dog->head_defn[idx].y, dog->head_defn[idx].z);
      MatrixStack::translate(dog->head_defn[idx].x, dog->head_defn[idx].y, dog->head_defn[idx].z);

      glRotatef(dog->head_defn[idx].rot_angle, 0.0f, 0.0f, 1.0f);
      MatrixStack::rotate(dog->head_defn[idx].rot_angle, 0.0f, 0.0f, 1.0f);

      glScalef(dog->head_defn[idx].sc_x, dog->head_defn[idx].sc_y, dog->head_defn[idx].sc_z);
      MatrixStack::scale(dog->head_defn[idx].sc_x, dog->head_defn[idx].sc_y, dog->head_defn[idx].sc_z);

	if(idx == 2)
	  {
	    curAmbient = GL_AMBIENT_RUBBER;
	    curDiffuse = GL_DIFFUSE_RUBBER;
	    curSpecular = GL_SPECULAR_RUBBER;
	    curShininess = GL_SHININESS_RUBBER;
	    curEmission = GL_EMISSION_RUBBER;
	  }
	else 
	  {
	    curAmbient = GL_AMBIENT_COPPER;
	    curDiffuse = GL_DIFFUSE_COPPER;
	    curSpecular = GL_SPECULAR_COPPER;
	    curShininess = GL_SHININESS_COPPER;
	    curEmission = GL_EMISSION_COPPER;
	  }
	      
	glMaterialfv(GL_FRONT, GL_AMBIENT, curAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, curDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, curSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, curShininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, curEmission);

      gluSphere(dog->head_defn[idx].sphereMesh, 0.05f, 10, 10);

      glPopMatrix(); MatrixStack::popMatrix();
      ++idx;
    }

  glPopMatrix(); MatrixStack::popMatrix();
}

void Dog::drawEyes()
{
  int idx = 0;
  glPushMatrix(); MatrixStack::pushMatrix();

  while(idx < NUM_EYE_SPHERES)
    {
      glPushMatrix(); MatrixStack::pushMatrix();
      glTranslatef(dog->eyes_defn[idx].x, dog->eyes_defn[idx].y, dog->eyes_defn[idx].z);
      MatrixStack::translate(dog->eyes_defn[idx].x, dog->eyes_defn[idx].y, dog->eyes_defn[idx].z);

      glScalef(dog->eyes_defn[idx].sc_x, dog->eyes_defn[idx].sc_y, dog->eyes_defn[idx].sc_z);
      MatrixStack::scale(dog->eyes_defn[idx].sc_x, dog->eyes_defn[idx].sc_y, dog->eyes_defn[idx].sc_z);
     
      // Pupils are on odd indices of the array
      if(idx % 2 == 0)
	{
	  glMaterialfv(GL_FRONT, GL_AMBIENT, GL_AMBIENT_EYE);
	  glMaterialfv(GL_FRONT, GL_DIFFUSE, GL_DIFFUSE_EYE);
	  glMaterialfv(GL_FRONT, GL_SPECULAR, GL_SPECULAR_EYE);
	  glMaterialfv(GL_FRONT, GL_SHININESS, GL_SHININESS_EYE);
	  glMaterialfv(GL_FRONT, GL_EMISSION, GL_EMISSION_EYE);
	}
      else
	{
	  glMaterialfv(GL_FRONT, GL_AMBIENT, GL_AMBIENT_RUBBER);
	  glMaterialfv(GL_FRONT, GL_DIFFUSE, GL_DIFFUSE_RUBBER);
	  glMaterialfv(GL_FRONT, GL_SPECULAR, GL_SPECULAR_RUBBER);
	  glMaterialfv(GL_FRONT, GL_SHININESS, GL_SHININESS_RUBBER);
	  glMaterialfv(GL_FRONT, GL_EMISSION, GL_EMISSION_RUBBER);
	}
      
      gluSphere(dog->eyes_defn[idx].sphereMesh, 0.05f, 10, 10);

      glPopMatrix(); MatrixStack::popMatrix();
      ++idx;
    }

  glPopMatrix(); MatrixStack::popMatrix();
}

void Dog::rotateLeft()
{
  dog_angle -= 2.0f;
  rotateDirection();
}

void Dog::rotateRight()
{
  dog_angle += 2.0f;
  rotateDirection();
}
