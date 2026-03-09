#include "logging.c"
#include "fft_complex_inputs.c"
#include "testing_randomisation.c"




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

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;



int main() {
  // complex test = {4294967295, 255};
   

  printf("Hello World!\n");
  int w = 512, h = 512;

  u32* write_buffer = malloc(w * h * 4 * sizeof(u32));
  // u64* buffer64 = (u64*)buffer; // Treat it as 64-bit pairs
  u32 dingus = get_random_u32();

  for (int pixel_n = 0; pixel_n < w * h; pixel_n++) {
    // buffer[pixel_n * 4 + 0] = get_random_u32();
    // buffer[pixel_n * 4 + 1] = get_random_u32();
    // buffer[pixel_n * 4 + 2] = get_random_u32();
    // buffer[pixel_n * 4 + 3] = get_random_u32();
    write_buffer[pixel_n * 4 + 0] = 100;
    write_buffer[pixel_n * 4 + 1] = 200;
    write_buffer[pixel_n * 4 + 2] = 300;
    write_buffer[pixel_n * 4 + 3] = 500;
  }

  printf("data generation done!\n");
  write_rgba_uint32_exr("uint32_output.exr", w, h, write_buffer);
  free(write_buffer); 


  u32* read_buffer = read_rgba_uint32_exr("uint32_output.exr", 512, 512);
  log_u32b(read_buffer[0]);
  log_u32b(read_buffer[1]);
  log_u32b(read_buffer[2]);
  log_u32b(read_buffer[3]);
  return 0;
}

