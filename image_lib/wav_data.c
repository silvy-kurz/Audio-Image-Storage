typedef enum {
  16B_MONO,  
  16B_STEREO,  
  24B_MONO,  
  24B_STEREO,  
} adi_sample_type_t

typedef struct {
    i16 left;
    i16 right;
} adi_spl_st16_t;

typedef struct {
    i16 value;
} adi_spl_mo16_t;

typedef struct __attribute__((packed)){
    i8 left_bytes[3];
    i8 right_bytes[3];
} adi_spl_st24_t;

typedef struct __attribute__((packed)){
    i8 value_bytes[3];
} adi_spl_mo24_t;

typedef struct {
  adi_sample_type_t type;
  int sample_count;
  void *sample_buffer;
} adi_samples_t;



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

