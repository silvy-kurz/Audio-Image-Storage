#include <openexr.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

void write_rgba_uint32_exr(const char *filename, int width, int height, const afi_pixel_u32_t *rgba_pixels)
{
  int pixel_count = width * height;
  int channel_count = 4;

  size_t pixel_stride = sizeof(u32);
  size_t channel_size = pixel_count * pixel_stride; 
  size_t total_size = channel_count * channel_size;

  u8 *data = malloc(total_size);

  for (int i = 0; i < width * height; i++) {

    memcpy(&data[i * 4 + channel_size * 0], &rgba_pixels[i].a, 4);
    memcpy(&data[i * 4 + channel_size * 1], &rgba_pixels[i].b, 4);
    memcpy(&data[i * 4 + channel_size * 2], &rgba_pixels[i].g, 4);
    memcpy(&data[i * 4 + channel_size * 3], &rgba_pixels[i].r, 4);
  }


  exr_context_initializer_t ctxtinit = EXR_DEFAULT_CONTEXT_INITIALIZER;
  exr_context_t myfile;
  exr_result_t rv = exr_start_write(&myfile, filename, EXR_WRITE_FILE_DIRECTLY, &ctxtinit);
  if (rv != EXR_ERR_SUCCESS) abort();

  int partidx;
  rv = exr_add_part(myfile, "RGBA", EXR_STORAGE_SCANLINE, &partidx);
  if (rv != EXR_ERR_SUCCESS) abort();

  exr_initialize_required_attr_simple(myfile, 0, width, height, EXR_COMPRESSION_PIZ);
  if (rv != EXR_ERR_SUCCESS) abort();

  exr_add_channel(myfile, 0, "R", EXR_PIXEL_UINT, 0, 1, 1);
  exr_add_channel(myfile, 0, "G", EXR_PIXEL_UINT, 0, 1, 1);
  exr_add_channel(myfile, 0, "B", EXR_PIXEL_UINT, 0, 1, 1);
  exr_add_channel(myfile, 0, "A", EXR_PIXEL_UINT, 0, 1, 1);


  rv = exr_write_header(myfile);

  if (rv != EXR_ERR_SUCCESS) abort();



    int nchunks = 0;
    exr_get_chunk_count(myfile, 0, &nchunks);

    int lines_per_chunk = 0;
    exr_get_scanlines_per_chunk(myfile, 0, &lines_per_chunk);


    size_t line_stride = pixel_stride * width;
    size_t chunk_stride = line_stride * lines_per_chunk;
    printf("chunk_stride = %zu\n", chunk_stride);
    printf("lines_per_chunk = %d\n", lines_per_chunk);
    printf("nchunks = %d\n", nchunks);
    size_t step = 0;
    exr_chunk_info_t cinfo;
    exr_encode_pipeline_t encoder;


    bool first = true;
    
    //======================================= ENCODE ===========================================

    for (int y = 0; y < nchunks; y++) {
        rv = exr_write_scanline_chunk_info(myfile, 0, y * lines_per_chunk, &cinfo);
        if (rv != EXR_ERR_SUCCESS) printf(" scanline chunk info failed at chunk %d!\n", y);
        
        if (first) {
            rv = exr_encoding_initialize(myfile, 0, &cinfo, &encoder);
            if (rv != EXR_ERR_SUCCESS) printf("huge error at chunk init %d!\n", y);

        } else  {
        rv = exr_encoding_update(myfile, 0, &cinfo, &encoder);
        if (rv != EXR_ERR_SUCCESS) printf("huge error at chunk update %d!\n", y);
        }     
        // cinfo.start_y = lines_per_chunk * y;
        printf("chunk %d start_y=%d height=%d\n",
              y,
              cinfo.start_y,
              cinfo.height);

        step = chunk_stride * y;

        encoder.channels[0].encode_from_ptr = &data[channel_size * 0 + step];
        encoder.channels[1].encode_from_ptr = &data[channel_size * 1 + step];
        encoder.channels[2].encode_from_ptr = &data[channel_size * 2 + step];
        encoder.channels[3].encode_from_ptr = &data[channel_size * 3 + step];

        encoder.channels[0].user_data_type = EXR_PIXEL_UINT;
        encoder.channels[1].user_data_type = EXR_PIXEL_UINT;
        encoder.channels[2].user_data_type = EXR_PIXEL_UINT;
        encoder.channels[3].user_data_type = EXR_PIXEL_UINT;


        encoder.channels[0].user_pixel_stride = pixel_stride;
        encoder.channels[1].user_pixel_stride = pixel_stride;
        encoder.channels[2].user_pixel_stride = pixel_stride;
        encoder.channels[3].user_pixel_stride = pixel_stride;

        encoder.channels[0].user_line_stride = line_stride;
        encoder.channels[1].user_line_stride = line_stride;
        encoder.channels[2].user_line_stride = line_stride;
        encoder.channels[3].user_line_stride = line_stride;

        if (first) {
          rv = exr_encoding_choose_default_routines(myfile, 0, &encoder);
          if (rv != EXR_ERR_SUCCESS) printf("huge error at chunk routine %d!\n", y);
        }
        rv = exr_encoding_run(myfile, 0, &encoder);
        if (rv != EXR_ERR_SUCCESS) printf("encoding run failed at chunk %d with error %d\n", y, rv);

        first = false;
    }



    free(data);
    exr_encoding_destroy(myfile, &encoder);
    exr_finish(&myfile);
}

