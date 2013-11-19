/**
 * Samuel Sweet
 * CS 5610-1 Assignment 2
 *
 * Convenience methods for organizing all
 * shader programs (implementations).
 */

#include "shader.hpp"

// Static refs
ShaderSystem::ShaderProgramNames ShaderSystem::current_program;
ShaderSystem::ShaderMap shader_programs;

ShaderSystem::ShaderSystem()
{
  current_program = PHONG;
}

ShaderSystem::~ShaderSystem()
{
  delete instance;
}

// Reads a shader file and returns a pointer to the
// char array representing it.
char* ShaderSystem::textFileRead(const char *fileName) 
{
  char* text = NULL;
	
  // Open the file to read only.
  if (fileName != NULL) {
    FILE *file = fopen(fileName, "rt");
	  
    if (file != NULL) {
      fseek(file, 0, SEEK_END);
      int count = ftell(file);
      rewind(file);
            
      if (count > 0) {
	text = (char*)malloc(sizeof(char) * (count + 1));
	count = fread(text, sizeof(char), count, file);
	text[count] = '\0';
      }
      fclose(file);
    }
  }

  return text;
}

// Checks compile/link information from a given shader program.
void ShaderSystem::validateShader(GLuint shader, const char* file) {
  const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  GLsizei length = 0;
    
  // Fill buffer with log data
  glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
  if (length > 0)
    printf("Shader %i (%s) compile error: %s\n", shader, file, buffer);
}

// Checks for linker errors in a shader program.
void ShaderSystem::validateProgram(GLuint program, const char* vs_name, const char* fs_name) 
{
  const unsigned int BUFFER_SIZE = 512;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  GLsizei length = 0;
  
  memset(buffer, 0, BUFFER_SIZE);
  glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
  if (length > 0)
    printf("Shader program %d (%s, %s) link error: %s\n", program, vs_name, fs_name, buffer);
  
  glValidateProgram(program);
  GLint status;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
  if (status == GL_FALSE) {
    printf("Error validating shader %d (%s, %s).\n", program, vs_name, fs_name);
  }
}

// Begins reading vertex and fragment shader, and defines shader program.
void ShaderSystem::initShader(const char *vsFile, const char *fsFile, ShaderSystem::ShaderProgramNames which) {
  // Pointers to our vars, so we only
  // change the ones we need
  ShaderProgram new_program;

  new_program.shader_vp = glCreateShader(GL_VERTEX_SHADER);
  new_program.shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
  
  // Read the program text files
  const char* vsText = textFileRead(vsFile);
  const char* fsText = textFileRead(fsFile);	
  
  // Errors? If so, print 'em
  if (vsText == NULL) perror("OOPS! Vertex shader couldn't be read.");
  if (fsText == NULL) perror("OOPS! Fragment shader couldn't be read.");
  if (vsText == NULL || fsText == NULL) return;
  
  glShaderSource(new_program.shader_vp, 1, &vsText, 0);
  glShaderSource(new_program.shader_fp, 1, &fsText, 0);
  
  glCompileShader(new_program.shader_vp);
  validateShader(new_program.shader_vp, vsFile);
  glCompileShader(new_program.shader_fp);
  validateShader(new_program.shader_fp, fsFile);
  
  new_program.shader_id = glCreateProgram();
  glAttachShader(new_program.shader_id, new_program.shader_vp);
  glAttachShader(new_program.shader_id, new_program.shader_fp);
  glLinkProgram(new_program.shader_id);
  validateProgram(new_program.shader_id, vsFile, fsFile);

  shader_programs[which] = new_program;
}

// Destroys both shader programs.
void ShaderSystem::destroyShader(void) 
{
  glDetachShader(shader_id, shader_fp);
  glDetachShader(shader_id, shader_vp);
    
  glDeleteShader(shader_fp);
  glDeleteShader(shader_vp);
  glDeleteProgram(shader_id);
}
