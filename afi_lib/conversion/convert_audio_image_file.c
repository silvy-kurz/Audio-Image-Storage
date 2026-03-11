#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

afi_result_t wav_to_exr(char *wav_file_name, char *exr_name) {
  afi_result_t rv;
  afi_wav_t wav_data = {0};

  printf("%d\n", read_wav(wav_file_name, &wav_data));
  log_wav_data(&wav_data);

  afi_samples_t *audio_samples = NULL;
  map_wav_data_samples(&wav_data, &audio_samples);
  log_samples(audio_samples, 10);

  afi_pixels_t *audio_pixels = NULL;
  rv = map_samples_to_pixels(audio_samples, &audio_pixels);
  if (rv != AFI_SUCCESS) {
      printf("Sample to Pixel Mapping Failed!\n");
  }
  log_pixels(audio_pixels, 100);
 //TODO: Implement Function to Determine Image Size from Pixel Count, Current just Works for Current Sample Count 
  int w = 147, h = 150;
  write_rgba_uint32_exr("exr_name", w, h, audio_pixels->pixel_buffer);
  free(wav_data.sampled_data);
  free(audio_samples->sample_buffer);
  free(audio_samples);
  return AFI_SUCCESS;
}

