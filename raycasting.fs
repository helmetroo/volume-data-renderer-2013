/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Raycasting Fragment Shader
 */

#version 120

#define MAX_MARCHES 256

uniform sampler2D backBoundingVol;

uniform sampler3D volumeTexture;

varying vec2 bufferTexCoords;
varying vec3 boxTexCoords;

void main()
{
  const float step_size = 0.1;

  // Calculate ray direction (back to front).
  vec3 origin = boxTexCoords;
  vec4 exitPoint = texture2D(backBoundingVol, bufferTexCoords);
  
  vec3 rayDirec = exitPoint.rgb - origin;
  rayDirec = normalize(rayDirec);
  vec3 rayDelta = step_size * rayDirec;

  // Begin ray at origin
  vec3 ray = origin;

  // Begin marching
  vec4 final_color = vec4(0.0);
  float final_alpha = 0.0;
  for(int i = 0; i < MAX_MARCHES; ++i)
    {
      ray += rayDelta;
      vec4 sample_color = texture3D(volumeTexture, origin);
      float sample_alpha = sample_color.a*step_size;

      // Back to front color blending
      final_color += (1.0 - final_alpha)*sample_color*sample_alpha; // and intensity?
      final_alpha += sample_alpha;
      
      if(final_alpha > 1.0)
	break;
    }

  gl_FragColor = final_color;
}
