#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h> 


void log_u32b(u32 number) {
    printf("%" PRIu32 "\n", number);
}

void log_u16b(u16 number) {
    printf("%" PRIu16 "\n", number);
}




void log_wav_data(adi_wav_t *wav_data) {
  printf("Total File Size : ");
  log_u32b(wav_data->total_file_size);

  printf("Formatting Chunk Size : ");
  log_u32b(wav_data->format_block_size);

  printf("Audio Format : ");
  log_u32b(wav_data->audio_format);

  printf("Channel Count : ");
  log_u32b(wav_data->channel_count);

  printf("Sample Rate : ");
  log_u32b(wav_data->sample_rate);

  printf("Bytes Per Second : ");
  log_u32b(wav_data->bytes_per_second);

  printf("Bytes Per Block : ");
  log_u32b(wav_data->bytes_per_block);

  printf("Bit Depth : ");
  log_u32b(wav_data->bit_depth);

  printf("Raw Data Size : ");
  log_u32b(wav_data->data_size);

}
