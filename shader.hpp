/**
 * Samuel Sweet
 * CS 5610-1 Project
 *
 * Singleton class representing shader programs
 * this application can use.
 */

#ifndef __SHADER_H
#define __SHADER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if ( (defined(__MACH__)) && (defined(__APPLE__)) )   
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#else
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <map>

class ShaderSystem
{
 public:
  ~ShaderSystem();
  
  // Constants for each program used
  enum ShaderProgramNames {
    PASSTHROUGH = 0,
    RAYCASTING  = 1,
    OUTPUT      = 2
  };

  struct ShaderProgram
  {
    GLuint shader_id;
    GLuint shader_vp;
    GLuint shader_fp;

    ShaderProgram(GLuint _shader_id = 0, GLuint _shader_vp = 0, GLuint _shader_fp = 0)
      : shader_id(_shader_id), shader_vp(_shader_vp), shader_fp(_shader_fp) {} 
  };

  inline static ShaderSystem* getInstance(void)
  {
    instance = new ShaderSystem();
    return instance;
  }

  // Amalgamated method to read files and initialize shaders
  void initShader(const char *vsFile, const char *fsFile, ShaderSystem::ShaderProgramNames which);
  
  // Tells OpenGL to use a particular shader program
  inline static void useShader(ShaderSystem::ShaderProgramNames which) 
  {
    current_program = which;
    useCurrentShader();
  }

  inline static void useCurrentShader(void) 
  {
    glUseProgram(currentShaderId());
  }

  // ID for reaching uniforms
  static inline GLuint currentShaderId(void) 
  {
    return shader_programs[current_program].shader_id;
  }
  
  // Extract a "ref" to a uniform in current program
  static inline GLint getUniformFromCurrentShader(const char* name)
  {
    return glGetUniformLocation(currentShaderId(), name);
  }

  static inline GLint getAttributeFromCurrentShader(const char* name)
  {
    return glGetAttribLocation(currentShaderId(), name);
  }

  void destroyShader(void);

 private:
  ShaderSystem();
    
  static ShaderProgramNames current_program;
  static std::map<ShaderProgramNames, ShaderProgram> shader_programs;

  static ShaderSystem* instance;

  char* textFileRead(const char* fileName);
  void validateShader(GLuint shader, const char* file);
  void validateProgram(GLuint program, const char* vs_name, const char* fs_name);
};

#endif
