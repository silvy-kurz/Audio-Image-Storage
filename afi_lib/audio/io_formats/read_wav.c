#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

afi_result_t get_chunk_count(FILE *file, u32 total_file_size, int *count_out) {
  u32 bytes_scanned = 12;
  u32 current_chunk_size;
  int chunk_count = 1;

  while (bytes_scanned < total_file_size) {
    fseek(file, 4, SEEK_CUR); 
    if (fread(&current_chunk_size, 1, sizeof(current_chunk_size), file) < sizeof(current_chunk_size)) {

      printf("Chunk Read Failed\n");
      return AFI_FILE_OPEN_FAILED;
    }

    fseek(file, current_chunk_size, SEEK_CUR);
    bytes_scanned += current_chunk_size + 8;
    chunk_count += 1;
  }

  *count_out = chunk_count;
  return AFI_SUCCESS;
}

afi_chunk_type_t get_chunk_type(char *chunk_id) {
    if (memcmp(chunk_id, "RIFF", 4) == 0) {
        return AFI_MASTER_RIFF;
    } else if (memcmp(chunk_id, "fmt ", 4) == 0) {
        return AFI_FORMAT;
    } else if (memcmp(chunk_id, "data", 4) == 0) {
        return AFI_DATA;
    } else {
        return AFI_EXTRA;
    }
}

afi_result_t load_file_chunks(FILE *file, int chunk_count, afi_chunk_container_t **chunks_out) {
  afi_chunk_container_t *chunks = malloc(sizeof(afi_chunk_container_t));
  afi_riff_chunk_t *chunk_buffer = malloc(chunk_count * sizeof(afi_riff_chunk_t));
  
  fseek(file, 4, SEEK_SET);
  chunk_buffer[0].type = AFI_MASTER_RIFF;
  memcpy(chunk_buffer[0].id, "RIFF", 4);
  fread(&chunk_buffer[0].size, 1, 4, file);
  chunk_buffer[0].data = malloc(4);
  fread(chunk_buffer[0].data, 1, 4, file);

    
  chunks->count = chunk_count;
  chunks->data = chunk_buffer;

  int chunk_index;
  size_t chunk_data_size = 8;
  u8 chunk_data[chunk_data_size];

  char chunk_id[4];
  u32 chunk_size;

  for (chunk_index = 1; chunk_index < chunk_count; chunk_index++) {
    if (fread(chunk_data, 1, chunk_data_size, file) < chunk_data_size) {
      printf("Chunk Read Failed\n");
      return AFI_FILE_OPEN_FAILED;
    }

    memcpy(&chunk_id, chunk_data, 4);
    memcpy(&chunk_size, chunk_data + 4, 4);

    memcpy(chunk_buffer[chunk_index].id, &chunk_id, 4);
    chunk_buffer[chunk_index].type = get_chunk_type(chunk_id);
    chunk_buffer[chunk_index].size = chunk_size;
    chunk_buffer[chunk_index].data = malloc(chunk_size);

    if (fread(chunk_buffer[chunk_index].data, 1, chunk_size, file) < chunk_size) {
      printf("Chunk Read Failed\n");
      return AFI_FILE_OPEN_FAILED;
    }
  }

  *chunks_out = chunks;
  return AFI_SUCCESS;
}

afi_result_t get_audio_format(u8 format, afi_audio_format_t *format_out) {
  switch (format) {
    case 1 : {
      *format_out = AFI_PCM_INTEGER; 
      return AFI_SUCCESS;
    }
    case 3 : {
      printf("IEE 754 Float Audio Not Yet Implemented\n");
      return AFI_TYPE_NOT_SUPPORTED;
    }
    default : return AFI_FILE_FORMAT_WRONG;
  }
}

afi_result_t parse_chunks(afi_chunk_container_t *chunks, afi_wav_t *wav_data) {
  afi_riff_chunk_t *chunk_buffer = chunks->data;
  int chunk_index;
  int data_chunks_parsed = 0;
  int format_chunks_parsed = 0;
  
  for (chunk_index = 0; chunk_index < chunks->count; chunk_index++) {
    switch (chunk_buffer[chunk_index].type) {
      case AFI_FORMAT : {
        if (chunk_buffer[chunk_index].size != 16) {
          printf("Non standard Format Size not Implemented\n");
          return AFI_TYPE_NOT_SUPPORTED;
        }

        u8 *format_data = chunk_buffer[chunk_index].data;

        u16 audio_format;
        memcpy(&audio_format, format_data, 2);
        get_audio_format(audio_format, &wav_data->audio_format);
         
        memcpy(&wav_data->channel_count, format_data + 2, 2);
        memcpy(&wav_data->sample_rate, format_data + 4, 4);
        memcpy(&wav_data->bytes_per_frame, format_data + 12, 2);
        memcpy(&wav_data->bit_depth, format_data + 14, 2);
        
        format_chunks_parsed += 1;
        break;
      }
      case AFI_DATA : {
        memcpy(&wav_data->raw_data_size, &chunk_buffer[chunk_index].size, 4);
        wav_data->raw_data = chunk_buffer[chunk_index].data;
        
        data_chunks_parsed += 1;
        break;
      }
      default : break;
    }
  }
  
  if (data_chunks_parsed < 1) {
    return AFI_FILE_FORMAT_WRONG;
  } 
  if (data_chunks_parsed > 1) {
    return AFI_TYPE_NOT_SUPPORTED;
  } 
  if (data_chunks_parsed != 1) {
    return AFI_FILE_FORMAT_WRONG;
  } 
  
  wav_data->chunks = chunks; 

  return AFI_SUCCESS;
}

afi_result_t read_wav(char *filename, afi_wav_t** data_out) {
  afi_wav_t *data = malloc(sizeof(afi_wav_t));

  FILE *fp = fopen(filename, "rb");
  if (!fp) {
  printf("file pointer null\n");
  return AFI_FILE_OPEN_FAILED;
  }
  u8 master_riff_data[12];

  if (fread(master_riff_data, 1, 12, fp) < 12) {
    printf("File Read Failed\n");
    return AFI_FILE_OPEN_FAILED;
  }

  u32 riff_file_size;
  memcpy(&riff_file_size, master_riff_data + 4, 4);

  int chunk_count;
  get_chunk_count(fp, riff_file_size + 8, &chunk_count);
  
  afi_chunk_container_t *wav_chunks = NULL;
  load_file_chunks(fp, chunk_count, &wav_chunks);
  fclose(fp);

  parse_chunks(wav_chunks, data);
  log_wav_data("", data);

  *data_out = data;
  return AFI_SUCCESS;
}
