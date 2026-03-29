#include <bsd/stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

i32
get_random_32b_int (i32 low, i32 high)
{
  return low + arc4random_uniform (high - low + 1);
}

u64
get_random_u64 ()
{
  u64 result;
  arc4random_buf (&result, sizeof (result));
  return result;
}

u32
get_random_u32 ()
{
  u32 result;
  arc4random_buf (&result, sizeof (result));
  return result;
}

u8
get_random_u8 ()
{
  u8 result;
  arc4random_buf (&result, sizeof (result));
  return result;
}

float
get_random_unit_interval ()
{
  float r = (float)arc4random () / (float)UINT32_MAX;

  // return low + r * (high - low);
  return r;
}

float
get_random_float (float low, float high)
{
  float r = (float)arc4random () / (float)UINT32_MAX;

  return low + r * (high - low);
}
