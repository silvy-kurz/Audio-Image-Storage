typedef enum afi_audio_format_e {
  AFI_PCM_INTEGER,
  AFI_IEE_754_FLOAT
} afi_audio_format_t;

// RIFF SPECIFIC 

typedef enum afi_chunk_type_e {
  AFI_MASTER_RIFF,
  AFI_FORMAT,
  AFI_DATA,  
  AFI_EXTRA
} afi_chunk_type_t;

typedef struct afi_riff_chunk_s {
  afi_chunk_type_t type;  
  char id[4];
  u32 size;
  u8 *data;
} afi_riff_chunk_t;

typedef struct afi_chunk_container_s {
  int count;
  afi_riff_chunk_t *data;
} afi_chunk_container_t;

// WAV SPECIFIC
typedef struct afi_wav_s {
  afi_audio_format_t audio_format;
  u16 channel_count;
  u32 sample_rate;
  u16 bytes_per_frame;
  u16 bit_depth;

  u32 raw_data_size;
  u8* raw_data;

  afi_chunk_container_t *chunks;

} afi_wav_t;

// LOGGING
void log_chunk_type(char *front, afi_riff_chunk_t *chunk) {
  switch (chunk->type) {
    case 0 : {
      printf("%s%s\n", front, "Master Riff Chunk"); 
      break;
    }
    case 1 : {
      printf("%s%s\n", front, "Format Chunk"); 
      break;
    }
    case 2 : {
      printf("%s%s\n", front, "Data Chunk"); 
      break;
    }
    case 3 : {
      printf("%s%s", front, "Extra Chunk with ID "); 
      log_char_array("", chunk->id, 4);
      break;
    }
  }
}
void log_chunks(afi_chunk_container_t *chunks) {
  int chunk_count = chunks->count;
  int chunk_index;
      
  log_u32("Total Chunk Count : ", chunk_count);
  for (chunk_index = 0; chunk_index < chunk_count; chunk_index++) {
    printf("Chunk %d\n", chunk_index);
    log_chunk_type("  Chunk Type : ", &chunks->data[chunk_index]);
    log_u32("  Chunk Size : ", chunks->data[chunk_index].size);
    printf("  Chunk Data Stored at %p\n", chunks->data[chunk_index].data); 
    printf("\n");
  }
}

void log_wav_data(afi_wav_t *wav_data) {
  printf("Loaded WAV File:");
  log_u16(" Audio Format : ", wav_data->audio_format);
  log_u16(" Channel Count : ", wav_data->channel_count);
  log_u32(" Sample Rate : ", wav_data->sample_rate);
  log_u32(" Bytes Per Frame : ", wav_data->bytes_per_frame);
  log_u16(" Bit Depth : ", wav_data->bit_depth);
  log_u32(" Raw Data Size : ", wav_data->raw_data_size);
  printf(" Sample Data Stored at %p\n",wav_data->raw_data); 
  printf(" Wav Chunks:\n");
  log_chunks(wav_data->chunks);
}
