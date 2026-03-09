#include <openexr.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


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


typedef struct {
  u64 real;
  u64 imag;
} complex;

void write_rgba_uint32_exr(const char* filename, int width, int height, u32* rgba_pixels)
{
  exr_context_initializer_t ctxtinit = EXR_DEFAULT_CONTEXT_INITIALIZER;

  exr_context_t myfile;

  exr_result_t rv = exr_start_write(&myfile, filename, EXR_WRITE_FILE_DIRECTLY, &ctxtinit);

  if (rv != EXR_ERR_SUCCESS)

  {
    abort();
  }

    /* add RGBA channels */
    exr_attr_chlist_t chlist;
    memset(&chlist, 0, sizeof(chlist));

    exr_add_channel(myfile, 0, "R", EXR_PIXEL_UINT, 1, 0, 0);
    exr_add_channel(myfile, 0, "G", EXR_PIXEL_UINT, 1, 0, 0);
    exr_add_channel(myfile, 0, "B", EXR_PIXEL_UINT, 1, 0, 0);
    exr_add_channel(myfile, 0, "A", EXR_PIXEL_UINT, 1, 0, 0);


    /* set image size */
    exr_attr_box2i_t dataWindow = {0, 0, width - 1, height - 1};
    exr_attr_box2i_t displayWindow = dataWindow;
    
    exr_set_data_window(myfile, 0, &dataWindow);
    exr_set_display_window(myfile, 0, &displayWindow);
    exr_set_compression(myfile, 0, EXR_COMPRESSION_PIZ);

    // /* allocate planar buffers */
    u32* r = malloc(sizeof(u32) * width * height);
    u32* g = malloc(sizeof(u32) * width * height);
    u32* b = malloc(sizeof(u32) * width * height);
    u32* a = malloc(sizeof(u32) * width * height);

    /* convert interleaved -> planar */
    for (int i = 0; i < width * height; i++)
    {
        r[i] = rgba_pixels[i * 4 + 0];
        g[i] = rgba_pixels[i * 4 + 1];
        b[i] = rgba_pixels[i * 4 + 2];
        a[i] = rgba_pixels[i * 4 + 3];
    }


  
    //
    // /* describe pixel buffers */
    // exr_image_t img;
    // memset(&img, 0, sizeof(img));
    //
    // img.num_channels = 4;
    // img.width = width;
    // img.height = height;
    //
    // img.channels[0].name = "R";
    // img.channels[0].pixel_type = EXR_PIXEL_UINT;
    // img.channels[0].pixels = r;
    //
    // img.channels[1].name = "G";
    // img.channels[1].pixel_type = EXR_PIXEL_UINT;
    // img.channels[1].pixels = g;
    //
    // img.channels[2].name = "B";
    // img.channels[2].pixel_type = EXR_PIXEL_UINT;
    // img.channels[2].pixels = b;
    //
    // img.channels[3].name = "A";
    // img.channels[3].pixel_type = EXR_PIXEL_UINT;
    // img.channels[3].pixels = a;
    //
    // /* write pixels */
    // ret = exr_write_image(ctxt, &img);
    //
    // /* cleanup */
    // free(r);
    // free(g);
    // free(b);
    // free(a);

    exr_finish(&myfile);
}

  
