// Core Functionality
#include "core/base_types.c"
#include "core/logging.c"

// Usage Specific Core Functionality 
#include "audio/audio_core.c"
#include "audio/formats_core.c"

#include "visual/visual_core.c"
#include "visual/formats_core.c"

#include "conversion/conversion_core.c"

// File Format Code
#include "audio/io_formats/read_wav.c"
#include "audio/io_formats/write_wav.c"

#include "visual/io_formats/write_exr.c"
#include "visual/io_formats/read_exr.c"

// File Format to Universal Data Code
#include "visual/format_to_pixels.c"
#include "visual/pixels_to_format.c"
#include "audio/format_to_frames.c"
#include "audio/frames_to_format.c"

// Universal Data Medium Conversion Code
#include "conversion/audio_to_visual.c"
#include "conversion/visual_to_audio.c"

// Final Pipeline Code
#include "conversion/convert_audio_image_file.c"
#include "conversion/convert_image_audio_file.c"

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
  afi_result_t pipeline_success = wav_to_exr("inputs/sin_test_stereo.wav", "outputs/sin_test_stereo.exr");
  if (pipeline_success == AFI_SUCCESS) {
    printf("\n");
    printf("===================================\n");
    printf("Audio Successfully Converted to Image\n");
    return 0;

  } else {
    printf("Audio to Image Conversion Failed!\n");
    return -1;
  }
}




