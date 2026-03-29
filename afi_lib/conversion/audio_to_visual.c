
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

afi_result_t
map_frames_to_pixels (afi_frames_t *frames, afi_pixels_t **pixels_out)
{
  afi_pixels_t *pixels = malloc (sizeof (afi_pixels_t));
  int frame_count = frames->frame_count;

  int pixel_index;
  int frame_index;

  switch (frames->type)
    {
    case MONO_16:
      {

        return AFI_TYPE_NOT_SUPPORTED;
      }
    case STEREO_16:
      {
        afi_frm_st16_t *frame_buffer = (afi_frm_st16_t *)frames->frame_buffer;
        int frames_per_pixel = 4;

        int full_pixels_count = frame_count / frames_per_pixel;
        int left_over_frames = frame_count % frames_per_pixel;
        int padding_pixels = (left_over_frames != 0);

        int pixel_count = full_pixels_count + padding_pixels;
        size_t pixel_buffer_size = sizeof (afi_pixel_u32_t) * pixel_count;
        afi_pixel_u32_t *pixel_buffer = malloc (pixel_buffer_size);

        if (pixel_buffer == NULL)
          {
            return AFI_MALLOC_FAILED;
          }

        for (pixel_index = 0; pixel_index < full_pixels_count; pixel_index++)
          {
            frame_index = pixel_index * frames_per_pixel;

            pixel_buffer[pixel_index].r
                = (u32)(frame_buffer[frame_index].left_sample | (frame_buffer[frame_index].right_sample << 16));
            pixel_buffer[pixel_index].g
                = (u32)(frame_buffer[frame_index + 1].left_sample | (frame_buffer[frame_index + 1].right_sample << 16));
            pixel_buffer[pixel_index].b
                = (u32)(frame_buffer[frame_index + 2].left_sample | (frame_buffer[frame_index + 2].right_sample << 16));
            pixel_buffer[pixel_index].a
                = (u32)(frame_buffer[frame_index + 3].left_sample | (frame_buffer[frame_index + 3].right_sample << 16));
          }

        for (int p = 0; p < padding_pixels; p++)
          {
            frame_index = full_pixels_count * frames_per_pixel;

            afi_pixel_u32_t last_pixel = { 0 };
            u32 *channels = (u32 *)&last_pixel;

            for (int s = 0; s < left_over_frames; s++)
              {
                channels[s] = (u32)(frame_buffer[frame_index + s].left_sample
                                    | (frame_buffer[frame_index + s].right_sample << 16));
              }

            pixel_buffer[full_pixels_count] = last_pixel;
          }

        pixels->type = RGBA_32;
        pixels->pixel_count = pixel_count;
        pixels->pixel_buffer = pixel_buffer;

        break;
      }
    case MONO_24:
      {

        return AFI_TYPE_NOT_SUPPORTED;
      }
    case STEREO_24:
      {

        return AFI_TYPE_NOT_SUPPORTED;
      }
    }

  *pixels_out = pixels;
  return AFI_SUCCESS;
}
