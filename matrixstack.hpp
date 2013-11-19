/*
 * Samuel Sweet
 * CS 5610-1 Assignment 3
 *
 * Duplicates the functionality of OpenGL's matrix stack,
 * but allows us access to world and view matrices, which is dope.
 */


#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <stack>

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shader.hpp"

// Typedef
typedef glm::mat3 Matrix3;
typedef glm::mat4 Matrix4;
typedef glm::vec3 Vector3;

typedef std::stack<Matrix4> Matrix4Stack;

class MatrixStack
{
public:
  MatrixStack();
  ~MatrixStack();

  enum MatrixTypes
    {
      WORLD,
      VIEW,
      PROJECTION,
      TEXTURE
    };

  inline static void matrixMode(MatrixTypes type) {
    switch(type) 
      {
      case MatrixStack::MatrixTypes::WORLD:
	current_stack = &worldMatrixStack;
	current_matrix = &currentWorldMatrix;
	break;
	
      case MatrixStack::MatrixTypes::VIEW:
	current_stack = &viewMatrixStack;
	current_matrix = &currentViewMatrix;
	break;
	
      case MatrixStack::MatrixTypes::PROJECTION:
	current_stack = &projectionMatrixStack;
	current_matrix = &currentProjectionMatrix;
	break;
	
      case MatrixStack::MatrixTypes::TEXTURE:
	current_stack = &textureMatrixStack;
	current_matrix = &currentTextureMatrix;
	break;
      }
  }

  inline static void loadIdentity(void)
  {
    *current_matrix = Matrix4(1.0f);
  }

  inline static void translate(GLfloat x, GLfloat y, GLfloat z)
  {
    *current_matrix = glm::translate(*current_matrix, Vector3(x, y, z));
  }

  inline static void scale(GLfloat x, GLfloat y, GLfloat z)
  {
    *current_matrix = glm::scale(*current_matrix, Vector3(x, y, z));
  }

  inline static void rotate(GLfloat rot_angle, GLfloat x, GLfloat y, GLfloat z)
  {
    *current_matrix = glm::rotate(*current_matrix, rot_angle, Vector3(x, y, z));
  }

  inline static void perspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
  { 
    *current_matrix *= glm::perspective(fov, aspect, near, far);
  }

  inline static void lookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat lookx, GLfloat looky, GLfloat lookz, GLfloat upx, GLfloat upy, GLfloat upz)
  {
    *current_matrix = glm::lookAt(Vector3(eyex, eyey, eyez), 
				       Vector3(lookx, looky, lookz), 
				       Vector3(upx, upy, upz));
  }

  inline static void pushMatrix(void)
  {
    // OpenGL always pushes a copy of the top matrix to the stack,
    // so naturally we do the same
    current_stack->push(*current_matrix);
  }

  inline static void popMatrix(void)
  {
    *current_matrix = current_stack->top();
    current_stack->pop();
  }

  inline static void bindWorldMatrix(void)
  {
    glUniformMatrix4fv(ShaderSystem::getUniformFromCurrentShader(WORLD_MATRIX_SHADER_NAME),
		       1, GL_FALSE, glm::value_ptr(currentWorldMatrix));
  }

  inline static void bindViewMatrix(void)
  {
    glUniformMatrix4fv(ShaderSystem::getUniformFromCurrentShader(VIEW_MATRIX_SHADER_NAME),
		       1, GL_FALSE, glm::value_ptr(currentViewMatrix));
  }

  inline static void bindInverseViewMatrix(void)
  {
    viewInverse = glm::inverse(currentViewMatrix);
    glUniformMatrix4fv(ShaderSystem::getUniformFromCurrentShader(INVERSE_VIEW_MATRIX_SHADER_NAME),
		       1, GL_FALSE, &viewInverse[0][0]);
  }
  
  inline static void bindProjectionMatrix(void)
  {
    glUniformMatrix4fv(ShaderSystem::getUniformFromCurrentShader(PROJECTION_MATRIX_SHADER_NAME),
		       1, GL_FALSE, &currentProjectionMatrix[0][0]);
  }
  
  inline static void bindTextureMatrix(void)
  {
    glUniformMatrix4fv(ShaderSystem::getUniformFromCurrentShader(TEXTURE_MATRIX_SHADER_NAME),
		       1, GL_FALSE, &currentTextureMatrix[0][0]);
  }
  

private:
  static Matrix4Stack* current_stack;
  static Matrix4* current_matrix;

  static Matrix4 viewInverse;
    
  static Matrix4Stack worldMatrixStack;
  static Matrix4Stack viewMatrixStack;
  static Matrix4Stack projectionMatrixStack;
  static Matrix4Stack textureMatrixStack;

  static Matrix4 currentWorldMatrix;
  static Matrix4 currentViewMatrix;
  static Matrix4 currentProjectionMatrix;
  static Matrix4 currentTextureMatrix;

  static const char* WORLD_MATRIX_SHADER_NAME;
  static const char* VIEW_MATRIX_SHADER_NAME;
  static const char* PROJECTION_MATRIX_SHADER_NAME;
  static const char* TEXTURE_MATRIX_SHADER_NAME;
  static const char* INVERSE_VIEW_MATRIX_SHADER_NAME;
};

#endif
