#include <openexr.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


u32* read_rgba_uint32_exr(const char* filename, int width, int height)
{
    exr_context_initializer_t ctxtinit = EXR_DEFAULT_CONTEXT_INITIALIZER;
    exr_context_t myfile;

    exr_result_t rv = exr_start_read(&myfile, filename, &ctxtinit);
    if (rv != EXR_ERR_SUCCESS) {
        printf("read init failed\n");
    }

    rv = exr_test_file_header(filename, &ctxtinit);
    if (rv == EXR_ERR_SUCCESS)
        printf("read init successful, and header correct\n");
    else
        printf("read init successful, but header incorrect\n");


    /* -----------------------------------------
       Query image information
    ------------------------------------------ */

    exr_attr_box2i_t datawindow;
    exr_get_data_window(myfile, 0, &datawindow);

    //
    // int nchannels = 0;
    // exr_get_channel_count(myfile, 0, &nchannels);
    // printf("nchannels = %d\n", nchannels);
    //
    // printf("width=%d height=%d channels=%d\n", width, height, nchannels);
    //
    int pixel_count = width * height;
    size_t pixel_stride = sizeof(u32);

    size_t channel_size = pixel_count * pixel_stride;
    size_t total_size = channel_size * 4;
    //
    u8* data = malloc(total_size);
    //
    //
    // /* -----------------------------------------
    //    Chunk info
    // ------------------------------------------ */
    //
    int nchunks = 0;
    exr_get_chunk_count(myfile, 0, &nchunks);
    printf("nchunks = %d\n", nchunks);
    //
    int lines_per_chunk = 0;
    exr_get_scanlines_per_chunk(myfile, 0, &lines_per_chunk);
    //
    size_t line_stride  = pixel_stride * width;
    size_t chunk_stride = line_stride * lines_per_chunk;

    printf("lines_per_chunk=%d\n", lines_per_chunk);
    printf("nchunks=%d\n", nchunks);
    //
    //
    // /* -----------------------------------------
    //    Decode pipeline
    // ------------------------------------------ */
    //
    exr_chunk_info_t cinfo;
    exr_decode_pipeline_t decoder;

    bool first = true;

    for (int y = 0; y < nchunks; y++)
    {
        rv = exr_read_scanline_chunk_info(myfile, 0, y * lines_per_chunk, &cinfo);
        if (rv != EXR_ERR_SUCCESS)
            printf("chunk info failed at %d\n", y);

        if (first) {
            rv = exr_decoding_initialize(myfile, 0, &cinfo, &decoder);
            if (rv != EXR_ERR_SUCCESS)
                printf("decode init failed at chunk %d\n", y);
        } else {
            rv = exr_decoding_update(myfile, 0, &cinfo, &decoder);
            if (rv != EXR_ERR_SUCCESS)
                printf("decode update failed at chunk %d\n", y);
        }

        size_t step = chunk_stride * y;

        decoder.channels[0].decode_to_ptr = &data[channel_size * 0 + step];
        decoder.channels[1].decode_to_ptr = &data[channel_size * 1 + step];
        decoder.channels[2].decode_to_ptr = &data[channel_size * 2 + step];
        decoder.channels[3].decode_to_ptr = &data[channel_size * 3 + step];

        decoder.channels[0].user_data_type = EXR_PIXEL_UINT;
        decoder.channels[1].user_data_type = EXR_PIXEL_UINT;
        decoder.channels[2].user_data_type = EXR_PIXEL_UINT;
        decoder.channels[3].user_data_type = EXR_PIXEL_UINT;

        decoder.channels[0].user_pixel_stride = pixel_stride;
        decoder.channels[1].user_pixel_stride = pixel_stride;
        decoder.channels[2].user_pixel_stride = pixel_stride;
        decoder.channels[3].user_pixel_stride = pixel_stride;

        decoder.channels[0].user_line_stride = line_stride;
        decoder.channels[1].user_line_stride = line_stride;
        decoder.channels[2].user_line_stride = line_stride;
        decoder.channels[3].user_line_stride = line_stride;

        if (first) {
            rv = exr_decoding_choose_default_routines(myfile, 0, &decoder);
            if (rv != EXR_ERR_SUCCESS)
                printf("routine selection failed\n");
        }

        rv = exr_decoding_run(myfile, 0, &decoder);
        if (rv != EXR_ERR_SUCCESS)
            printf("decode run failed at chunk %d\n", y);

        first = false;
    }
    //
    //
    // /* -----------------------------------------
    //    Reassemble RGBA interleaved pixels
    // ------------------------------------------ */
    //
    u32 *rgba_pixels = malloc(pixel_count * 4 * sizeof(u32));

    for (int i = 0; i < pixel_count; i++)
    {
        memcpy(&rgba_pixels[i * 4 + 0], &data[channel_size * 0 + i * 4], 4);
        memcpy(&rgba_pixels[i * 4 + 1], &data[channel_size * 1 + i * 4], 4);
        memcpy(&rgba_pixels[i * 4 + 2], &data[channel_size * 2 + i * 4], 4);
        memcpy(&rgba_pixels[i * 4 + 3], &data[channel_size * 3 + i * 4], 4);
    }


    printf("first pixel:\n");


    /* cleanup */

    free(data);

    exr_decoding_destroy(myfile, &decoder);
    exr_finish(&myfile);
    return rgba_pixels;
}


void write_rgba_uint32_exr(const char* filename, int width, int height, const u32* rgba_pixels)
{
  int pixel_count = width * height;
  int channel_count = 4;

  size_t pixel_stride = sizeof(u32);
  size_t channel_size = pixel_count * pixel_stride; 
  size_t total_size = channel_count * channel_size;

  u8 *data = malloc(total_size);

  for (int i = 0; i < width * height; i++) {

    memcpy(&data[i * 4 + channel_size * 0], &rgba_pixels[i * 4 + 0], 4);
    memcpy(&data[i * 4 + channel_size * 1], &rgba_pixels[i * 4 + 1], 4);
    memcpy(&data[i * 4 + channel_size * 2], &rgba_pixels[i * 4 + 2], 4);
    memcpy(&data[i * 4 + channel_size * 3], &rgba_pixels[i * 4 + 3], 4);
  }

  printf("data[0] = %d\n", data[0]);

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

  printf("rgba_pixels[0]\n");
  log_u32b(rgba_pixels[0]);




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

  
