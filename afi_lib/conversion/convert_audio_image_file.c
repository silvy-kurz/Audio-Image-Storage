#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

afi_result_t wav_to_exr(char *wav_file_name, char *exr_name) {
  afi_result_t rv;
  afi_wav_t *wav_data = NULL;

  printf("%d\n", read_wav(wav_file_name, &wav_data));
  log_wav_data("", wav_data);

  afi_frames_t *audio_frames = NULL;
  map_wav_data_frames(wav_data, &audio_frames);
  log_frames("", audio_frames, 10);

  afi_pixels_t *audio_pixels = NULL;
  rv = map_frames_to_pixels(audio_frames, &audio_pixels);
  if (rv != AFI_SUCCESS) {
      printf("frame to Pixel Mapping Failed!\n");
  }
  log_pixels("", audio_pixels, 100);
 //TODO: Implement Function to Determine Image Size from Pixel Count, Current just Works for Current Frame Count 
  int w = 147, h = 150;
  write_rgba_uint32_exr(exr_name, w, h, audio_pixels->pixel_buffer);
  free(wav_data->raw_data);
  free(audio_frames->frame_buffer);
  free(audio_frames);
  return AFI_SUCCESS;
}

