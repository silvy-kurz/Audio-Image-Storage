#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


afi_result_t fill_sample_type(afi_wav_t *wav_data, afi_samples_t *sample_out) {
  switch (wav_data->bit_depth) {
    case 16:
      switch (wav_data->channel_count) {
        case 1: {
          sample_out->type = MONO_16;
          return AFI_SUCCESS;
        }

        case 2: {
          sample_out->type = STEREO_16;
          return AFI_SUCCESS;
        }

        default: return AFI_FILE_FORMAT_WRONG;
      }
    case 24:
      switch (wav_data->channel_count) {
        case 1: {
          sample_out->type = MONO_24;
          return AFI_SUCCESS;
        }

        case 2: {
          sample_out->type = STEREO_24;
          return AFI_SUCCESS;
        }
        default: return AFI_FILE_FORMAT_WRONG;
      }
      default: return AFI_FILE_FORMAT_WRONG;
  }
}



afi_result_t map_wav_data_samples(afi_wav_t *wav_data, afi_samples_t **out_samples) {
  int sample_count = wav_data->data_size / wav_data->bytes_per_block;
  size_t bytes_per_data_sample = wav_data->bytes_per_block;
  u8 *raw_data = wav_data->sampled_data;

  int data_index = 0;
  int sample_index;

  afi_samples_t *samples = malloc(sizeof(afi_samples_t));
  fill_sample_type(wav_data, samples);
  samples->sample_count = sample_count;

  switch (samples->type) {
    case MONO_16: {

      return AFI_TYPE_NOT_SUPPORTED;
    } 


    case STEREO_16: {
      int samples_size = sample_count * sizeof(afi_spl_st16_t);
      afi_spl_st16_t *samples_buffer = malloc(samples_size);

      if (samples_buffer == NULL) {
        return AFI_MALLOC_FAILED;
      }

      log_u32("Allocated Sample Buffer of Size : ", samples_size);

      for (sample_index = 0; sample_index < sample_count; sample_index++) {
        data_index = sample_index * bytes_per_data_sample;

        samples_buffer[sample_index].left  = (i16)(raw_data[data_index]   | (raw_data[data_index + 1] << 8));
        samples_buffer[sample_index].right = (i16)(raw_data[data_index + 2] | (raw_data[data_index + 3] << 8));
      }

      samples->sample_buffer = samples_buffer;
      break;

    } 
    case MONO_24: {

      return AFI_TYPE_NOT_SUPPORTED;
    }

    case STEREO_24: {

      return AFI_TYPE_NOT_SUPPORTED;
    }
   } 

  *out_samples = samples;

  return AFI_SUCCESS;
}


