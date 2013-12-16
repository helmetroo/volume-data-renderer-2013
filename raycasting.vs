/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Raycasting Shader
 */

#version 120

varying vec4 final_position;
varying vec3 box_tex_coords;

void main()
{
  box_tex_coords = vec3(gl_MultiTexCoord6);
  gl_Position = ftransform();
  final_position = gl_Position;
}
