// Core Universally Used Functionality
#include "core/base_types.c"
#include "core/logging.c"

// Usage Specific Struct Definitions
#include "audio/audio_data.c"
#include "audio/formats_data.c"

#include "visual/visual_data.c"
#include "visual/formats_data.c"

// Format IO Code
#include "audio/io_formats/read_wav.c"
#include "audio/io_formats/write_wav.c"

#include "visual/io_formats/write_exr.c"
#include "visual/io_formats/read_exr.c"

// Internal Representation Code
#include "visual/format_to_pixels.c"
#include "audio/format_to_samples.c"
#include "conversion/audio_to_visual.c"
#include "conversion/visual_to_audio.c"

//  Debug Code
#include "debug/testing_randomisation.c"


#include <openexr.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WIDTH 2000  
#define HEIGHT 2000 


int filler_image_gen() {
  int w = 512, h = 512;

  afi_pixel_u32_t* write_buffer = malloc(w * h * sizeof(afi_pixel_u32_t));

  for (int pixel_n = 0; pixel_n < w * h; pixel_n++) {
    write_buffer[pixel_n] = (afi_pixel_u32_t){get_random_u32(), get_random_u32(), get_random_u32(), get_random_u32()};
  }

  printf("data generation done!\n");
  write_rgba_uint32_exr("outputs/uint32_output.exr", w, h, write_buffer);
  free(write_buffer); 


  afi_pixel_u32_t* read_buffer = read_rgba_uint32_exr("outputs/uint32_output.exr", 512, 512);

  free(read_buffer);
  return 0;
}

int main() {
  afi_result_t rv;

  afi_wav_t wav_data = {0};

  printf("%d\n", read_wav("inputs/sin_test_stereo.wav", &wav_data));
  log_wav_data(&wav_data);

  afi_samples_t *audio_samples = NULL;
  map_wav_data_samples(&wav_data, &audio_samples);
  log_samples(audio_samples, 10);

  afi_pixels_t *audio_pixels = NULL;
  rv = map_samples_to_pixels(audio_samples, &audio_pixels);
  if (rv != AFI_SUCCESS) {
      printf("Sample to Pixel Mapping Failed!\n");
  }
  log_pixels(audio_pixels, 100);


  free(wav_data.sampled_data);
  free(audio_samples->sample_buffer);
  free(audio_samples);
}




