#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h> 


void log_u32b(char *front, u32 number) {
    printf("%s%" PRIu32 "\n",front, number);
}

void log_u16b(char *front, u16 number) {
    printf("%s%" PRIu16 "\n", front, number);
}




void log_wav_data(adi_wav_t *wav_data) {

  log_u32b("Total File Size : ", wav_data->total_file_size);
  log_u16b("Formatting Chunk Size : ", wav_data->format_block_size);
  log_u16b("Audio Format : ", wav_data->audio_format);
  log_u16b("Channel Count : ", wav_data->channel_count);
  log_u32b("Sample Rate : ", wav_data->sample_rate);
  log_u32b("Bytes Per Second : ", wav_data->bytes_per_second);
  log_u16b("Bytes Per Block : ", wav_data->bytes_per_block);
  log_u16b("Bit Depth : ", wav_data->bit_depth);
  log_u32b("Raw Data Size : ", wav_data->data_size);
  
}
