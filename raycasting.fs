/*
 * CS 5610-1 Project
 * Samuel Sweet
 *
 * Raycasting Fragment Shader
 */

#version 120

uniform sampler2D frontBoundingVol;
uniform sampler2D backBoundingVol;

uniform sampler3D outputVolume;

varying vec4 final_position;
varying vec3 box_tex_coords;

// Custom parameters
uniform float step;
uniform float intensity;

void main()
{
  // TODO change to uniform from app
  // Step size SHOULD NOT be very small or too big either.
  const float step_size = 0.15;
  const float intensity = 0.75;
  const int marches = 2048;

  // Calculate ray direction (back to front).
  // Indexing backfaces texture requires a conversion of
  // clipspace coordinates to normalized-device coords needed to index texture properly.
  vec2 buffer_tex_coords = ((final_position.xy/final_position.w) + 1.0) / 2.0;
  vec4 face_origin = texture2D(frontBoundingVol, buffer_tex_coords);
  vec4 destination = texture2D(backBoundingVol, buffer_tex_coords);
  
  // Compute direction
  vec3 ray_direction = (destination.xyz - face_origin.xyz);
  vec3 norm_ray_direction = normalize(ray_direction);
  vec3 rayDelta = step_size * norm_ray_direction;

  // Begin ray at origin, distance computation
  vec3 ray = face_origin.xyz;
  float ray_length = length(ray_direction);
  float delta_length = length(rayDelta);
  float current_length_acc = 0.0;

  // Begin marching
  vec4 final_color = vec4(0.0);
  float final_alpha = 0.0;
  for(int march = 0; march < marches; ++march)
    {
      // March the delta amount along the ray
      vec4 sample_color = texture3D(outputVolume, ray);
      float sample_alpha = sample_color.a*step_size;

      // Back to front color blending to output color at a pixel
      final_color += (1.0 - final_alpha)*final_color + sample_color*sample_alpha;
      final_alpha += sample_alpha;

      // Front to back color blending to output color at a pixel
      //final_color = sample_color + final_color*final_alpha*(1.0 - sample_alpha);
      //final_alpha = sample_alpha + final_alpha*(1.0 - sample_alpha);
      //final_color.a = final_alpha;
      //final_color *= intensity;

      // Move to next position
      ray += rayDelta;
      current_length_acc += delta_length;

      // A stopping criterion for opacity and length (non-isovalue)
      if(final_alpha > 1.0 || current_length_acc > ray_length) {
	break;
      }
    }

  final_color *= intensity;
  gl_FragColor = final_color;
}
