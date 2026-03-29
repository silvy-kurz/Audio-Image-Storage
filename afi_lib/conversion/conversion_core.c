typedef enum afi_ver_afvf_s
{
  AFI_VER_000,
  AFI_VER_010
} afi_ver_afvf_t;

typedef enum afi_file_encoded_e
{
  AFI_FILE_WAV,
  AFI_FILE_RIFF
} afi_file_encoded_t;

typedef struct afi_sections_s
{
  u32 section_count;
  u32 *section_offsets;
  u8 *metadata;
} afi_sections_t;

typedef struct afi_audio_images_s
{
  afi_ver_afvf_t version;
  afi_file_encoded_t file_format;

  afi_sections_t *file_non_data_sections;
  u32 frames_original_section_index;

  afi_frames_t *file_data;

} afi_audio_pixels_t;

// ============ SPECIFICATION OF AFVF FORMAT VERSION 010 ============
//    u32 version
//    u32 file_format
//
//    u32 section_count
//    u32 frames_original_section_index
//    u32[section_count] metadata_section_offsets;
//    u8[metadata_section_offsets[section_count - 1]] metadata
//
//    u32 frame_count
//    u32 frame_type
//    u8[frame_count * sizeof(frame_type)] data
//
//
//
//
//
//
