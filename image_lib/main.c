// Universally Used Type Conversions and Enums
#include "base_types.c"

// Usage Specific Struct Definitions
#include "wav_data.c"
#include "exr_data.c"

// Logging Debug/Code
#include "logging.c"
#include "testing_randomisation.c"

// Functionality Implementing Code
#include "wav_read.c"
#include "wav_write.c"
#include "exr_read.c"
#include "exr_write.c"




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


int main_fft() {
  int w = 512, h = 512;

  adi_pixel_u32_t* write_buffer = malloc(w * h * sizeof(adi_pixel_u32_t));

  for (int pixel_n = 0; pixel_n < w * h; pixel_n++) {
    write_buffer[pixel_n] = (adi_pixel_u32_t){get_random_u32(), get_random_u32(), get_random_u32(), get_random_u32()};
  }

  printf("data generation done!\n");
  write_rgba_uint32_exr("outputs/uint32_output.exr", w, h, write_buffer);
  free(write_buffer); 


  adi_pixel_u32_t* read_buffer = read_rgba_uint32_exr("outputs/uint32_output.exr", 512, 512);

  free(read_buffer);
  return 0;
}

int main() {
  adi_wav_t wav_data = {0};

  printf("%d\n", read_wav("inputs/sin_test_stereo.wav", &wav_data));
  log_wav_data(&wav_data);
  adi_sample_2c16_t *wav_samples = NULL;
  cast_raw_wav_2c16(&wav_data, &wav_samples);
  for (int i = 0; i < 100; i++) {
    printf("Sample %d\n", i);
    log_sample_2c16("", wav_samples[i]);
  }
  free(wav_data.sampled_data);
  free(wav_samples);
  main_fft();
}




