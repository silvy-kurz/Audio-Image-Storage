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

void log_spl_st16_t(char *front, afi_spl_st16_t sample) {
  printf("%s", front);
  log_i16("  Left : ", sample.left);
  log_i16("  Right : ", sample.right);
}

