#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>



void log_key_part(u64 part, int part_index) {
  int byte;
  printf("  Part_%d: ",part_index);
  for (byte = 0; byte < 8; byte++) {
    printf("%02X|", (part >> 8 * byte) & 0xFF);
    
  }
  printf("\n");
}

