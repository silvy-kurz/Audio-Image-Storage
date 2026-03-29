typedef enum afi_frame_type_e
{
  MONO_16,
  STEREO_16,
  MONO_24,
  STEREO_24,
} afi_frame_type_t;

typedef struct afi_frm_st16_s
{
  i16 left_sample;
  i16 right_sample;
} afi_frm_st16_t;

typedef struct afi_frm_mo16_s
{
  i16 sample;
} afi_frm_mo16_t;

typedef struct afi_frm_st24_s
{
  u8 left_sample_bytes[3];
  u8 right_sample_bytes[3];
} __attribute__ ((packed)) afi_frm_st24_t;

typedef struct afi_frm_mo24_s
{
  u8 value_bytes[3];
} __attribute__ ((packed)) afi_frm_mo24_t;

typedef struct afi_frames_s
{
  afi_frame_type_t type;
  int frame_count;
  void *frame_buffer;
} afi_frames_t;

void
log_frm_mo16_t (char *front, afi_frm_mo16_t frame)
{
  printf ("%s", front);
  log_i16 ("  Sample : ", frame.sample);
}

void
log_frm_st16_t (char *front, afi_frm_st16_t frame)
{
  printf ("%s", front);
  log_i16 ("  Left Sample : ", frame.left_sample);
  printf ("%s", front);
  log_i16 ("  Right Sample : ", frame.right_sample);
}

void
log_frm_mo24_t (char *front, afi_frm_st16_t frame)
{
  printf ("%s", front);
  log_i16 ("  Left Sample : ", frame.left_sample);
  printf ("%s", front);
  log_i16 ("  Right Sample : ", frame.right_sample);
}

void
log_frm_st24_t (char *front, afi_frm_st16_t frame)
{
  printf ("%s", front);
  log_i16 ("  Left : ", frame.left_sample);
  printf ("%s", front);
  log_i16 ("  Right : ", frame.right_sample);
}

afi_result_t
log_frames (char *front, afi_frames_t *frames, int amount)
{
  printf ("%sFrames Container has %d Frames\n", front, frames->frame_count);

  switch (frames->type)
    {
    case MONO_16:
      {

        return AFI_TYPE_NOT_SUPPORTED;
      }
    case STEREO_16:
      {
        printf ("%sframes are 16 Bit Stereo\n", front);
        afi_frm_st16_t *frame_buffer = (afi_frm_st16_t *)frames->frame_buffer;
        if (amount > 0)
          {
            printf ("%sPrinting First %d Frames: \n\n", front, amount);
            for (int i = 0; i < amount; i++)
              {
                printf ("%s  Frame %d:\n", front, i);
                printf ("%s\n", front);
                log_frm_st16_t ("  ", frame_buffer[i]);
                printf (" \n");
              };
          }
        return AFI_SUCCESS;
      }
    case MONO_24:
      {

        return AFI_TYPE_NOT_SUPPORTED;
      }
    case STEREO_24:
      {

        return AFI_TYPE_NOT_SUPPORTED;
      }
    default:
      return AFI_TYPE_NOT_SUPPORTED;
    }
}
