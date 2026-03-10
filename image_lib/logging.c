#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h> 


void log_u32b(u32 number) {
    printf("%" PRIu32 "\n", number);
}

void log_u16b(u16 number) {
    printf("%" PRIu16 "\n", number);
}


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

