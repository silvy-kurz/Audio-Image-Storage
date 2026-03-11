
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>



afi_result_t map_samples_to_pixels(afi_samples_t *samples, afi_pixels_t **pixels_out) {
  afi_pixels_t *pixels = malloc(sizeof(afi_pixels_t));
  int sample_count = samples->sample_count;

  int pixel_index;
  int sample_index;

  switch (samples->type) {
    case MONO_16 : {

      return AFI_TYPE_NOT_SUPPORTED;
    } 
    case STEREO_16 : {
      afi_spl_st16_t *sample_buffer = (afi_spl_st16_t *)samples->sample_buffer;
      int samples_per_pixel = 4;

      int full_pixels_count = sample_count / samples_per_pixel; 
      int left_over_samples = sample_count % samples_per_pixel;
      int padding_pixels = (left_over_samples != 0);

      int pixel_count = full_pixels_count + padding_pixels;
      size_t pixel_buffer_size = sizeof(afi_pixel_u32_t) * pixel_count;
      afi_pixel_u32_t *pixel_buffer = malloc(pixel_buffer_size);

      if (pixel_buffer == NULL) {
        return AFI_MALLOC_FAILED;
      }

      for (pixel_index = 0; pixel_index < full_pixels_count; pixel_index++) {
        sample_index = pixel_index * samples_per_pixel;

        pixel_buffer[pixel_index].r = (u32)(sample_buffer[sample_index].left   | (sample_buffer[sample_index].right << 16));
        pixel_buffer[pixel_index].g = (u32)(sample_buffer[sample_index + 1].left   | (sample_buffer[sample_index + 1].right << 16));
        pixel_buffer[pixel_index].b = (u32)(sample_buffer[sample_index + 2].left   | (sample_buffer[sample_index + 2].right << 16));
        pixel_buffer[pixel_index].a = (u32)(sample_buffer[sample_index + 3].left   | (sample_buffer[sample_index + 3].right << 16));
      }

      for (int p = 0; p < padding_pixels; p++) {
          sample_index = full_pixels_count * samples_per_pixel;
          
          afi_pixel_u32_t last_pixel = {0}; 
          u32 *channels = (u32 *)&last_pixel; 

          for (int s = 0; s < left_over_samples; s++) {
              channels[s] = (u32)(sample_buffer[sample_index + s].left | (sample_buffer[sample_index + s].right << 16));
          }

          pixel_buffer[full_pixels_count] = last_pixel;
      }

      pixels->type = RGBA_32;
      pixels->pixel_count = pixel_count;
      pixels->pixel_buffer = pixel_buffer;
      
      break;
    }
    case MONO_24 : {

      return AFI_TYPE_NOT_SUPPORTED;
    }
    case STEREO_24 : {
    
      return AFI_TYPE_NOT_SUPPORTED;
    }
  }
  
  *pixels_out = pixels;
  return AFI_SUCCESS;
}
