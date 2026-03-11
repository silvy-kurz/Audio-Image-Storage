typedef struct afi_wav_s {
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
} afi_wav_t;

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

