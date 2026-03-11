#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h> 


void log_u32(char *front, u32 number) {
    printf("%s%" PRIu32 "\n",front, number);
}

void log_u16(char *front, u16 number) {
    printf("%s%" PRIu16 "\n", front, number);
}

void log_i16(char *front, i16 number) {
    printf("%s%" PRId16 "\n", front, number);
}


void log_spl_st16_t(char *front, afi_spl_st16_t sample) {
  printf("%s", front);
  log_i16("  Left : ", sample.left);
  log_i16("  Right : ", sample.right);
}


void log_wav_data(afi_wav_t *wav_data) {

  log_u32("Total File Size : ", wav_data->total_file_size);
  log_u16("Formatting Chunk Size : ", wav_data->format_block_size);
  log_u16("Audio Format : ", wav_data->audio_format);
  log_u16("Channel Count : ", wav_data->channel_count);
  log_u32("Sample Rate : ", wav_data->sample_rate);
  log_u32("Bytes Per Second : ", wav_data->bytes_per_second);
  log_u16("Bytes Per Block : ", wav_data->bytes_per_block);
  log_u16("Bit Depth : ", wav_data->bit_depth);
  log_u32("Raw Data Size : ", wav_data->data_size);
  
}
