#include <openexr.h>
#include <SDL2/SDL.h>
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
        case 1:
          sample_out->type = MONO_16;
          return AFI_SUCCESS;

        case 2:
          sample_out->type = STEREO_16;
          return AFI_SUCCESS;

        default: 
          return AFI_FILE_FORMAT_WRONG;
      }

    case 24:
      switch (wav_data->channel_count) {
        case 1:
          sample_out->type = MONO_24;
          return AFI_SUCCESS;

        case 2:
          sample_out->type = STEREO_24;
          return AFI_SUCCESS;

        default: 
          return AFI_FILE_FORMAT_WRONG;
      }
    
      default: 
        return AFI_FILE_FORMAT_WRONG;
  }
}

afi_result_t load_and_check_header(FILE *file, u8 *header) {
  if (fread(header, 1, 44, file) < 44) {
    printf("File Read Failed\n");
    return AFI_FILE_OPEN_FAILED;
  }

  if (memcmp(header, "RIFF", 4) != 0) {
    printf("Not a RIFF File\n");
    return AFI_FILE_FORMAT_WRONG;
  } 
  else if (memcmp(header + 8, "WAVE", 4) != 0) {
    printf("RIFF file, but not a WAV file\n");
    return AFI_FILE_FORMAT_WRONG;
  } else if (memcmp(header + 12, "fmt", 3) != 0) {
    printf("File is Valid WAV, but Data Format is Incompatible\n");
    return AFI_FILE_FORMAT_WRONG;
  }

  return AFI_SUCCESS;
}


afi_result_t parse_header_metadata(u8 *header, afi_wav_t *wav_data) {
  u32 md_file_size;
  memcpy(&md_file_size, header + 4, 4);
  wav_data->total_file_size = md_file_size + 8U;
  u32 fmt_size;  
  memcpy(&fmt_size, header + 16, 4);
  if (fmt_size != 16) {
    printf("Format Size Irregular or Incompatible\n");
    log_u16("Got Format Size : ", fmt_size);
    return AFI_FILE_FORMAT_WRONG;
  } else {
    wav_data->format_block_size = fmt_size;
  }
   

  memcpy(&wav_data->audio_format, header + 20, 2);

  memcpy(&wav_data->channel_count, header + 22, 2);

  
  memcpy(&wav_data->sample_rate, header + 24, 4);

  memcpy(&wav_data->bytes_per_second, header + 28, 4);

  memcpy(&wav_data->bytes_per_block, header + 32, 2);

  memcpy(&wav_data->bit_depth, header + 34, 2);

  return AFI_SUCCESS;
}


afi_result_t parse_data_section(FILE *file, u8 *header, afi_wav_t *wav_data) {
  int assumed_data_id_offset = 36;

  if (memcmp(header + assumed_data_id_offset, "data", 4) != 0) {
    printf("data not found\n");
    return AFI_FILE_FORMAT_WRONG;
  }
   
  u32 md_data_size;
  memcpy(&md_data_size, header + assumed_data_id_offset + 4, 4);
  wav_data->data_size = md_data_size;

// TODO: ADD CHECKING OF METADATA DATA SIZE AGAINST TOTAL SIZE - DATA POSITION
//       FOR NOW JUST ASSUMING SIZE IS CORRECT
   
  int data_bytes_offset = assumed_data_id_offset + 8;
  fseek(file, data_bytes_offset, SEEK_SET);

  wav_data->sampled_data = malloc(wav_data->data_size);
  if (wav_data->sampled_data == NULL) {
    return AFI_MALLOC_FAILED;
  }
  
  printf("%zu bytes loaded\n", fread(wav_data->sampled_data, 1, wav_data->data_size, file));
  
  
  
  return AFI_SUCCESS;
}


afi_result_t read_wav(char *filename, afi_wav_t* data_out) {
  afi_result_t rv; 

  FILE *fp = fopen(filename, "rb");
  if (!fp) {
  printf("file pointer null\n");
  return AFI_FILE_OPEN_FAILED;
  }


  u8 meta_header[44];
  rv = load_and_check_header(fp, meta_header);
  if (rv != AFI_SUCCESS) {
    abort(); 
  } 

  parse_header_metadata(meta_header, data_out);
  parse_data_section(fp, meta_header, data_out);
  
  fclose(fp);
    

return AFI_SUCCESS;
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
