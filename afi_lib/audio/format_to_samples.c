#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


afi_result_t fill_frame_type(afi_wav_t *wav_data, afi_frames_t *frame_out) {
  switch (wav_data->bit_depth) {
    case 16:
      switch (wav_data->channel_count) {
        case 1: {
          frame_out->type = MONO_16;
          return AFI_SUCCESS;
        }

        case 2: {
          frame_out->type = STEREO_16;
          return AFI_SUCCESS;
        }

        default: return AFI_FILE_FORMAT_WRONG;
      }
    case 24:
      switch (wav_data->channel_count) {
        case 1: {
          frame_out->type = MONO_24;
          return AFI_SUCCESS;
        }

        case 2: {
          frame_out->type = STEREO_24;
          return AFI_SUCCESS;
        }
        default: return AFI_FILE_FORMAT_WRONG;
      }
      default: return AFI_FILE_FORMAT_WRONG;
  }
}



afi_result_t map_wav_data_frames(afi_wav_t *wav_data, afi_frames_t **out_frames) {
  afi_frames_t *frames = malloc(sizeof(afi_frames_t));
  fill_frame_type(wav_data, frames);

  int frame_count = wav_data->raw_data_size / wav_data->bytes_per_frame;
  size_t bytes_per_data_frame = wav_data->bytes_per_frame;
  u8 *raw_data = wav_data->raw_data;

  frames->frame_count = frame_count;

  int data_index;
  int frame_index;

  switch (frames->type) {
    case MONO_16: {

      return AFI_TYPE_NOT_SUPPORTED;
    } 


    case STEREO_16: {
      int frames_size = frame_count * sizeof(afi_frm_st16_t);
      afi_frm_st16_t *frames_buffer = malloc(frames_size);

      if (frames_buffer == NULL) {
        return AFI_MALLOC_FAILED;
      }

      log_u32("Allocated frame Buffer of Size : ", frames_size);

      for (frame_index = 0; frame_index < frame_count; frame_index++) {
        data_index = frame_index * bytes_per_data_frame;

        frames_buffer[frame_index].left_sample  = (i16)(raw_data[data_index]   | (raw_data[data_index + 1] << 8));
        frames_buffer[frame_index].right_sample = (i16)(raw_data[data_index + 2] | (raw_data[data_index + 3] << 8));
      }

      frames->frame_buffer = frames_buffer;
      break;

    } 
    case MONO_24: {

      return AFI_TYPE_NOT_SUPPORTED;
    }

    case STEREO_24: {

      return AFI_TYPE_NOT_SUPPORTED;
    }
   } 

  *out_frames = frames;

  return AFI_SUCCESS;
}


