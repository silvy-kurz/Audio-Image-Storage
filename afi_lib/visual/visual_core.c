typedef enum afi_pixel_type_e {
  RGBA_32,  
  RGBA_16F,  
  RGB_8,  
} afi_pixel_type_t;


typedef struct afi_pixel_u32_s{
  u32 r;
  u32 g;
  u32 b;
  u32 a;
} afi_pixel_u32_t;


typedef struct afi_pixels_s {
  afi_pixel_type_t type;
  int pixel_count;
  void *pixel_buffer;
} afi_pixels_t;



void log_pixel_u32(char *front, afi_pixel_u32_t pixel) {
  printf("%s", front);
  log_u32("  R: ", pixel.r);
  printf("%s", front);
  log_u32("  G: ", pixel.g);
  printf("%s", front);
  log_u32("  B: ", pixel.b);
  printf("%s", front);
  log_u32("  A: ", pixel.a);
}




afi_result_t log_pixels(char * front, afi_pixels_t *pixels, int amount) {
  printf("%sPixels Container has %d Pixels\n",front, pixels->pixel_count);

  switch (pixels->type) {
    case RGBA_32 : {
      printf("%sPixels are 32 Bit RGBA\n", front); 
      afi_pixel_u32_t *pixel_buffer = (afi_pixel_u32_t *)pixels->pixel_buffer;
      
      if (amount > 0) {
        printf("%sPrinting First %d Pixels: \n\n", front, amount);
        for (int i = 0; i < amount; i++) {
          printf("%s  Pixel %d:\n",front, i);
          printf("%s\n", front);
          log_pixel_u32("  ", pixel_buffer[i]);
          printf("\n");
        };
      }

      return AFI_SUCCESS;
    } 
    case RGBA_16F : {

      return AFI_TYPE_NOT_SUPPORTED;
    }
    case RGB_8 : {

      return AFI_TYPE_NOT_SUPPORTED;
    }
    default : return AFI_TYPE_NOT_SUPPORTED;
  }
}
