#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h> 


void log_u32(char *front, u32 number) {
    printf("%s%" PRIu32 "\n",front, number);
}

void log_u16(char *front, u16 number) {
    printf("%s%" PRIu16 "\n", front, number);
}

void log_i16(char *front, i16 number) {
    printf("%s%" PRId16 "\n", front, number);
}




