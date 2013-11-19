/**
 * Samuel Sweet
 * CS 5610-1 Assignment 3
 *
 * Represents the dog through "Point" objects
 * that define a sphere with given position, rotation and scale,
 * and a Dog object that represents each "Point" making up the dog.
 */

#ifndef DOG_H
#define DOG_H

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include <stdio.h>
#include <math.h>

#include "matrixstack.hpp"
#include "object.hpp"
#include "shader.hpp"
#include "texture.hpp"

// Units of the dog mesh
struct Point
{
  GLUquadricObj* sphereMesh;

  // Position
  float x;
  float y;
  float z;

  // Sphere scale
  float sc_x;
  float sc_y;
  float sc_z;

  // Rotation angle
  float rot_angle;

  static GLUquadricObj* initSphereMesh()
  {
    GLUquadricObj* sphereMesh = gluNewQuadric(); // Create A Pointer To The Quadric Object
    gluQuadricDrawStyle(sphereMesh, GLU_FILL);
    gluQuadricOrientation(sphereMesh, GLU_OUTSIDE);
    gluQuadricNormals(sphereMesh, GLU_SMOOTH); // Create Smooth Normals

    return sphereMesh;
  }

  // Creates a reference to a new sphere mesh.
  static void definePoint(Point* point_to_define, float x, float y, float z, 
		   float sc_x, float sc_y, float sc_z, float rot_angle)
  {
    point_to_define->sphereMesh = Point::initSphereMesh();
    point_to_define->x = x;
    point_to_define->y = y;
    point_to_define->z = z;
    
    point_to_define->sc_x = sc_x;
    point_to_define->sc_y = sc_y;
    point_to_define->sc_z = sc_z;

    point_to_define->rot_angle = rot_angle;
  }
};

// BEGIN dog definition
#define NUM_LEG_SPHERES 3
#define NUM_BODY_SPHERES 5
#define NUM_EYE_SPHERES 4
#define NUM_HEAD_SPHERES 5

class Dog : public Object
{
  // Represents the dog model.
 public:
  Dog();
  ~Dog();

  // Draw methods
  void draw(void);
  void drawBody(void);
  void drawFrontLegs(void);
  void drawBackLegs(void);
  void drawLeg(float leg_angle);
  void drawHead(void);
  void drawEyes(void);
  void drawTail(void);
  
  // Animation methods / interactions
  void rotateLeft(void); // k
  void rotateRight(void); // l
  // Resets the dog position to its orig. state
  inline void resetPosition(void)
  {
    pos_x = init_x;
    pos_y = init_y;
    pos_z = init_z;
  }

  inline void speedUp(void)
  {
    dog_speed += 0.0001f;
    rotateDirection();
  }

  inline void slowDown(void)
  {
    if(dog_speed > 0)
      dog_speed -= 0.0001f;

    rotateDirection();
  }

  void rotateDirection(void);

  void updateAnim(void);

 private:  
  // current info regarding dog
  float dog_speed;
  float dog_angle;
  float* dog_direc_vect;

  // About an origin
  float* BACK_L_LEG_ORIG;
  float* BACK_R_LEG_ORIG;
  float* FRONT_L_LEG_ORIG;
  float* FRONT_R_LEG_ORIG;

  float back_l_joint_angle;
  float back_r_joint_angle;
  float front_l_joint_angle;
  float front_r_joint_angle;
  float angle_time;
  
  struct DogModel
  {
    Point body_defn[NUM_BODY_SPHERES];
    Point head_defn[NUM_HEAD_SPHERES];
    Point eyes_defn[NUM_EYE_SPHERES];
    Point leg_defn [NUM_LEG_SPHERES];
  };

  DogModel* dog;
  ImageTexture* head_texture;
  
  // Degrees-to-radians conversion constant
  static const float RADS;
};

#endif // End header guard decl
