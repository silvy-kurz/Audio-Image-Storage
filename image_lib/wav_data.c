typedef enum afi_sample_type_e {
  MONO_16,  
  STEREO_16,  
  MONO_24,  
  STEREO_24,  
} afi_sample_type_t;

typedef struct afi_spl_st16_s {
    i16 left;
    i16 right;
} afi_spl_st16_t;

typedef struct afi_spl_mo16_s {
    i16 value;
} afi_spl_mo16_t;

typedef struct afi_spl_st24_s {
    i8 left_bytes[3];
    i8 right_bytes[3];
} __attribute__((packed)) afi_spl_st24_t;

typedef struct afi_spl_mo24_s{
    i8 value_bytes[3];
} __attribute__((packed)) afi_spl_mo24_t;

typedef struct afi_samples_s {
  afi_sample_type_t type;
  int sample_count;
  void *sample_buffer;
} afi_samples_t;



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

