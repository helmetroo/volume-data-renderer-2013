/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Raycasting Fragment Shader
 */

#version 120

// Will do uniform that can be set by application later.
#define MAX_MARCHES 256

uniform sampler2D back_bounding_volume;

uniform sampler3D volume_texture;

varying vec4 out_position;
varying vec3 box_tex_coords;

void main()
{
  // TODO change to uniform from app
  const float step_size = 0.1;

  // Calculate ray direction (back to front).
  // Indexing backfaces texture requires a conversion of
  // clipspace coordinates to normalized-device coords needed to index texture properly.
  vec2 buffer_tex_coords = (out_position.xy + 1.0) / 2.0;
  vec3 origin = box_tex_coords;
  vec4 destination = texture2D(back_bounding_volume, buffer_tex_coords);
  
  // Compute direction
  vec3 ray_direction = normalize(destination.xyz - origin);
  vec3 rayDelta = step_size * ray_direction;

  // Begin ray at origin
  vec3 ray = origin;

  // Begin marching
  vec4 final_color = vec4(0.0);
  float final_alpha = 0.0;
  for(int march = 0; march < MAX_MARCHES; ++march)
    {
      // March the delta amount along the ray
      vec4 sample_color = texture3D(volume_texture, ray);
      float sample_alpha = sample_color.a*step_size;

      // Back to front color blending to output color at a pixel
      final_color += (1.0 - final_alpha)*sample_color*sample_alpha;
      final_alpha += sample_alpha;

      // Move to next position
      ray += rayDelta;
    }

  gl_FragColor = final_color;
}
