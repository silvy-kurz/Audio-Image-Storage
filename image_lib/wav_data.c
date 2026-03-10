
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

