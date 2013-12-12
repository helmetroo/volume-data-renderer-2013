/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Raycasting Fragment Shader
 */

#version 120

uniform sampler2D back_bounding_volume;

uniform sampler3D volume_texture;

varying vec4 out_position;
varying vec3 box_tex_coords;

// Custom parameters
uniform float step;
uniform float intensity;

void main()
{
  // TODO change to uniform from app
  // Step size SHOULD NOT be very small or too big either.
  const float step_size = 0.25;
  const float intensity = 0.5;
  const int marches = 512;

  // Calculate ray direction (back to front).
  // Indexing backfaces texture requires a conversion of
  // clipspace coordinates to normalized-device coords needed to index texture properly.
  vec2 buffer_tex_coords = (out_position.xy/out_position.w + 1.0) / 2.0;
  vec3 origin = box_tex_coords;
  vec4 destination = texture2D(back_bounding_volume, buffer_tex_coords);
  
  // Compute direction
  vec3 ray_direction = normalize(origin - destination.xyz);
  vec3 rayDelta = step_size * ray_direction;

  // Begin ray at origin
  vec3 ray = destination.xyz;
  float len = length(ray_direction);
  float delta_len = length(rayDelta);
  float len_acc = 0.0;

  // Begin marching
  vec4 final_color = vec4(0.0);
  float final_alpha = 0.0;
  for(int march = 0; march < marches; ++march)
    {
      // March the delta amount along the ray
      vec4 sample_color = texture3D(volume_texture, ray);
      float sample_alpha = sample_color.a*step_size;

      // Back to front color blending to output color at a pixel
      final_color += (1.0 - final_alpha)*final_color + sample_color*sample_alpha;
      final_color *= intensity;

      // Move to next position
      ray += rayDelta;
      len_acc += delta_len;

      // A stopping criterion
      if(sample_alpha > 1.0) {
	break;
      }
    }

  gl_FragColor = final_color;
}
