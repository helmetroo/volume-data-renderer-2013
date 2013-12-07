/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Passthrough Fragment Shader
 */

varying vec2 screen_coords;

uniform sampler2D output_image;

void main()
{
  // Convert to normalized device coords, and we're done!
  gl_FragColor = texture2D(output_image, (screen_coords+1.0)/2.0);
}
