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

typedef enum {
  ADI_SUCCESS,
  ADI_FILE_OPEN_FAILED,
  ADI_FILE_FORMAT_WRONG,
  ADI_FILE_CORRUPTED,
  ADI_UNKNOWN_ERROR
} adi_success_code_t;


typedef struct {
  u32 total_file_size;
  u16 format_block_size;
  u16 audio_format;
  u16 channel_count;
  u32 sample_rate;
  u32 bytes_per_second;
  u16 bytes_per_block;
  u16 bit_depth;
  u32 data_size;
  u8* sampled_data;
} adi_wav_t;



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
    printf("Got Format Size ");
    log_u16b(fmt_size);
    return ADI_FILE_FORMAT_WRONG;
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
  log_u32b(wav_data->data_size);

// TODO: ADD CHECKING OF METADATA DATA SIZE AGAINST TOTAL SIZE - DATA POSITION
//       FOR NOW JUST ASSUMING SIZE IS CORRECT
   
  int data_bytes_offset = assumed_data_id_offset + 8;
  fseek(file, data_bytes_offset, SEEK_SET);

  wav_data->sampled_data = malloc(wav_data->data_size);
  if (wav_data->sampled_data == NULL) {
    printf("malloc failed\n");
  }
  
  printf("%zu bytes loaded\n", fread(wav_data->sampled_data, 1, wav_data->data_size, file));
  
  
  
  return ADI_SUCCESS;
}



adi_success_code_t read_wav() {
  FILE *fp = fopen("inputs/dingus.wav", "rb");
  if (!fp) {
  printf("file pointer null\n");
  return ADI_FILE_OPEN_FAILED;
  }
  adi_success_code_t rv; 


  u8 meta_header[44];
  rv = load_and_check_header(fp, meta_header);
  if (rv != ADI_SUCCESS) {
    abort(); 
  } 

  adi_wav_t wav_data = {0};
  parse_header_metadata(meta_header, &wav_data);
  parse_data_section(fp, meta_header, &wav_data);
  log_wav_data(wav_data);
  
  free(meta_header);
  fclose(fp);
    

return ADI_SUCCESS;
}
