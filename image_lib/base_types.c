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

typedef enum afi_result_e{
  AFI_SUCCESS,
  AFI_FILE_OPEN_FAILED,
  AFI_FILE_FORMAT_WRONG,
  AFI_FILE_CORRUPTED,
  AFI_TYPE_NOT_SUPPORTED,
  AFI_UNKNOWN_ERROR,
  AFI_MALLOC_FAILED,
} afi_result_t;

