#include <openexr.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>


afi_pixel_u32_t* read_rgba_uint32_exr(const char* filename, int width, int height)
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
     afi_pixel_u32_t* rgba_pixels = malloc(pixel_count * sizeof(afi_pixel_u32_t));

    for (int i = 0; i < pixel_count; i++)
    {
        memcpy(&rgba_pixels[i].a, &data[channel_size * 0 + i * 4], 4);
        memcpy(&rgba_pixels[i].b, &data[channel_size * 1 + i * 4], 4);
        memcpy(&rgba_pixels[i].g, &data[channel_size * 2 + i * 4], 4);
        memcpy(&rgba_pixels[i].a, &data[channel_size * 3 + i * 4], 4);
    }


    /* cleanup */

    free(data);

    exr_decoding_destroy(myfile, &decoder);
    exr_finish(&myfile);
    return rgba_pixels;
}

