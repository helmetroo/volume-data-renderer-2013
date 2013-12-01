/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Raycasting Fragment Shader
 */

#version 120

#define MAX_SAMPLES 256

uniform sampler2D frontBoundingVol;
uniform sampler2D backBoundingVol;

uniform sampler3D volumeTexture;

void main()
{
  const float stepSize = 0.1;

  // Calculate ray direction (back to front).
  vec3 origin = texture2D(frontBoundingVol, bufferTexCoords).rgb;
  vec4 exitPoint = texture2D(backBoundingVol, bufferTexCoords);

  vec3 rayDirec = exitPoint.rgb - origin;
  rayDirec = normalize(rayDirec);
  vec3 rayDelta = delta * rayDirec;

  // Begin ray at origin
  vec3 ray = origin;

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
