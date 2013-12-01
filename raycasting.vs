/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Raycasting Shader
 */

#version 120

varying vec2 bufferTexCoords;

void main()
{
  bufferTexCoords = vec2(gl_MultiTexCoord0);

  // No transforms necessary since the vert is in clip space.
  gl_Position = gl_Vertex;
}
