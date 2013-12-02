/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Raycasting Shader
 */

#version 120

varying vec2 bufferTexCoords;
varying vec3 boxTexCoords;

void main()
{
  bufferTexCoords = vec2(gl_MultiTexCoord0);
  boxTexCoords = vec2(gl_MultiTexCoord6);

  gl_Position = ftransform();
}
