/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Passthrough Vertex Shader
 */

varying vec2 screen_coords;

void main()
{
  screen_coords = vec2(gl_MultiTexCoord7);
  gl_Position = gl_Vertex;
}
