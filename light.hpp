#ifndef LIGHT_H
#define LIGHT_H

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shader.hpp"

typedef glm::mat4 Matrix4;
typedef glm::vec4 Vector4;
typedef glm::vec3 Vector3;

class Light
{
public:
  Light();
  Light(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
  ~Light();

  inline void setPos(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w) 
  {
    light_position[0] = light_xz_pos[0] = _x;
    light_position[1] = light_y_pos = _y;
    light_position[2] = light_xz_pos[1] = _z;
    light_position[3] = _w;
  }

  inline void bindLightColor(void) 
  {    
    glLightfv(GL_LIGHT0, GL_AMBIENT, DEFAULT_AMBIENT);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, DEFAULT_DIFFUSE);
    glLightfv(GL_LIGHT0, GL_SPECULAR, DEFAULT_SPECULAR);
  }

  inline void bindLightPos(void) 
  {
    light_position[0] = light_xz_pos[0];
    light_position[1] = light_y_pos;
    light_position[2] = light_xz_pos[1];
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  }

  inline void setViewMatrix(void)
  {
    view_matrix = glm::lookAt(Vector3(light_position[0], light_position[1], light_position[2]), 
		Vector3(0.0f, 0.0f, 0.0f), 
		Vector3(0.0f, 1.0f, 0.0f));
  }

  inline void setProjMatrix()
  {
    projection_matrix = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 500.0f);
  }

  inline void bindViewMatrix(void)
  {
    glUniformMatrix4fv(ShaderSystem::getUniformFromCurrentShader("lightViewMatrix"), 
		       1, GL_FALSE, &view_matrix[0][0]);
  }

  inline void bindProjMatrix(void)
  {
    glUniformMatrix4fv(ShaderSystem::getUniformFromCurrentShader("lightProjectionMatrix"), 
		       1, GL_FALSE, &projection_matrix[0][0]);
  }

  inline void bindModelViewMatrix(void)
  {
    glUniformMatrix4fv(ShaderSystem::getUniformFromCurrentShader("lightModelViewMatrix"), 
		       1, GL_FALSE, light_modelview_matrix);
  }

  inline GLfloat* xzRef(void) { return light_xz_pos; }
  inline GLfloat* yRef(void) { return &light_y_pos; }
  inline GLfloat* position(void) { return light_position; }
  inline GLfloat* modelViewMatrix(void) { return light_modelview_matrix; }

private:
  void initLightArrays(void);

  // lighting info
  const static GLfloat DEFAULT_AMBIENT[];
  const static GLfloat DEFAULT_DIFFUSE[];
  const static GLfloat DEFAULT_SPECULAR[];

  GLfloat* light_position;
  GLfloat* light_xz_pos;
  GLfloat  light_y_pos;

  GLfloat* light_modelview_matrix;

  Matrix4 view_matrix;
  Matrix4 projection_matrix;
};

#endif
