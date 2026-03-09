#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h> 

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

void log_u32b(u32 number) {
    printf("The value is: %" PRIu32 "\n", number);
}

typedef struct {
  u64 real;
  u64 imag;
} complex;

void log_complex(complex number) {
  int byte;
  printf("Complex Number: \n");
  printf("Real Part: ");
  for (byte = 0; byte < 8; byte++) {
    printf("%02X|", (u8)(number.real >> 8 * byte) & 0xFF);
    
  }
  printf("\n");

  printf("Imaginary Part: ");
  for (byte = 0; byte < 8; byte++) {
    printf("%02X|", (u8)(number.imag >> 8 * byte) & 0xFF);
    
  }
  printf("\n");
}

