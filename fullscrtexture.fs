/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Passthrough Fragment Shader
 */

varying vec2 screenCoords;
uniform sampler2D outputImage;

void main()
{
  gl_FragColor = texture2D(outputImage, screenCoords);
}
