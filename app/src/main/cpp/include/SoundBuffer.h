#ifndef __ARKANOID_SOUND_BUFFER__H__
#define __ARKANOID_SOUND_BUFFER__H__

#include "AssetStorage.h"

namespace native {

class SoundBuffer {
public:
  SoundBuffer(AssetStorage* assets, const char* filename);
  SoundBuffer(const char* filepath);
  virtual ~SoundBuffer();

  const char* getFilename() const;
  const char* getName() const;
  uint8_t* const getData() const;
  off_t getLength() const;

  virtual bool load();
  virtual void unload();

protected:
  virtual uint8_t* loadSound() = 0;

  enum class ReadMode : int {
    ASSETS = 0, FILESYSTEM = 1
  };

  ReadMode m_read_mode;
  AssetStorage* m_assets;
  char* m_filename;
  off_t m_length;
  uint8_t* m_data;
  int m_error_code;
};

/**
 * Error codes (SoundBuffer)
 *
 * 2021 - assets->open() failed
 * 2022 - data allocation failed from assets
 * 2023 - assets->read() failed
 * 2024 - fopen() failed
 * 2025 - data allocation failed from file
 * 2026 - fread() failed
 */

// ----------------------------------------------------------------------------
/// @brief Class allows to operate with WAV and PCM files.
class WAVSound : public SoundBuffer {
public:
  WAVSound(AssetStorage* assets, const char* filename);
  WAVSound(const char* filepath);
  virtual ~WAVSound();

  struct WAVHeader{
    char                RIFF[4];
    unsigned long       ChunkSize;
    char                WAVE[4];
    char                fmt[4];
    unsigned long       Subchunk1Size;
    unsigned short      AudioFormat;
    unsigned short      NumOfChan;
    unsigned long       SamplesPerSec;
    unsigned long       bytesPerSec;
    unsigned short      blockAlign;
    unsigned short      bitsPerSample;
    char                Subchunk2ID[4];
    unsigned long       Subchunk2Size;
  };

protected:
  uint8_t* loadSound() override final;
};

}

#endif  // __ARKANOID_SOUND_BUFFER__H__
