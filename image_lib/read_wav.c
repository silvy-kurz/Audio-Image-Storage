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


adi_success_code_t load_and_check_header(FILE *file, u8 *header) {
  if (fread(header, 1, 44, file) < 44) {
    printf("File Read Failed\n");
    return ADI_FILE_OPEN_FAILED;
  }

  if (memcmp(header, "RIFF", 4) != 0) {
    printf("Not a RIFF File\n");
    return ADI_FILE_FORMAT_WRONG;
  } 
  else if (memcmp(header + 8, "WAVE", 4) != 0) {
    printf("RIFF file, but not a WAV file\n");
    return ADI_FILE_FORMAT_WRONG;
  } else if (memcmp(header + 12, "fmt", 3) != 0) {
    printf("File is Valid WAV, but Data Format is Incompatible\n");
    return ADI_FILE_FORMAT_WRONG;
  }

  return ADI_SUCCESS;
}


adi_success_code_t parse_header_metadata(u8 *header, adi_wav_t *wav_data) {
  u32 md_file_size;
  memcpy(&md_file_size, header + 4, 4);
  wav_data->total_file_size = md_file_size + 8U;
  u32 fmt_size;  
  memcpy(&fmt_size, header + 16, 4);
  if (fmt_size != 16) {
    printf("Format Size Irregular or Incompatible\n");
    log_u16("Got Format Size : ", fmt_size);
    return ADI_FILE_FORMAT_WRONG;
  } else {
    wav_data->format_block_size = fmt_size;
  }
   

  memcpy(&wav_data->audio_format, header + 20, 2);

  memcpy(&wav_data->channel_count, header + 22, 2);

  
  memcpy(&wav_data->sample_rate, header + 24, 4);

  memcpy(&wav_data->bytes_per_second, header + 28, 4);

  memcpy(&wav_data->bytes_per_block, header + 32, 2);

  memcpy(&wav_data->bit_depth, header + 34, 2);

  return ADI_SUCCESS;
}


adi_success_code_t parse_data_section(FILE *file, u8 *header, adi_wav_t *wav_data) {
  int assumed_data_id_offset = 36;

  if (memcmp(header + assumed_data_id_offset, "data", 4) != 0) {
    printf("data not found\n");
    return ADI_FILE_FORMAT_WRONG;
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
    return ADI_MALLOC_FAILED;
  }
  
  printf("%zu bytes loaded\n", fread(wav_data->sampled_data, 1, wav_data->data_size, file));
  
  
  
  return ADI_SUCCESS;
}


adi_success_code_t read_wav(char *filename, adi_wav_t* data_out) {
  adi_success_code_t rv; 

  FILE *fp = fopen(filename, "rb");
  if (!fp) {
  printf("file pointer null\n");
  return ADI_FILE_OPEN_FAILED;
  }


  u8 meta_header[44];
  rv = load_and_check_header(fp, meta_header);
  if (rv != ADI_SUCCESS) {
    abort(); 
  } 

  parse_header_metadata(meta_header, data_out);
  parse_data_section(fp, meta_header, data_out);
  
  fclose(fp);
    

return ADI_SUCCESS;
}


adi_success_code_t cast_raw_wav_2c16(adi_wav_t* wav_data, adi_sample_2c16_t **out_samples) {
  int sample_count = wav_data->data_size / wav_data->bytes_per_block;
  int samples_size = sample_count * sizeof(adi_sample_2c16_t);
  u8 *raw_data = wav_data->sampled_data;

  adi_sample_2c16_t *samples = malloc(samples_size);
  if (samples == NULL) {
    return ADI_MALLOC_FAILED;
  }
  log_u32("Allocated Sample Buffer of Size : ", samples_size);

  int bytes_per_sample = wav_data->bytes_per_block;
  int data_index = 0;

  int sample_index;
  for (sample_index = 0; sample_index < sample_count; sample_index++) {
    data_index = sample_index * bytes_per_sample;

    samples[sample_index].left  = (int16_t)(raw_data[data_index]   | (raw_data[data_index + 1] << 8));
    samples[sample_index].right = (int16_t)(raw_data[data_index + 2] | (raw_data[data_index + 3] << 8));
  }
  
  *out_samples = samples;
  return ADI_SUCCESS;
}


