/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Passthrough Vertex Shader
 */


void main()
{
  gl_FrontColor = gl_Color;
  gl_Position = ftransform();
}
