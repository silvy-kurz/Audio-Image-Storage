#include "fft_complex_inputs.c"
#include "logging.c"
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
  complex test = {get_random_64b_int(), get_random_64b_int()};

  printf("Hello World!\n");
  int w = 512, h = 512;
  u32* buffer = malloc(w * h * 4 * sizeof(u32));

  for (int i = 0; i < w * h * 4; i++) {
      buffer[i] = 4200000000U; // Example: high-value 32-bit int
  }

  write_rgba_uint32_exr("output_uint32.exr", w, h, buffer);

  free(buffer); 
  log_complex(test);
  return 0;
}

