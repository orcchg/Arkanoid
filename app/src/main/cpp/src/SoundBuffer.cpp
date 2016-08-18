#include <algorithm>
#include <cstdio>
#include <cstring>
#include <libgen.h>

#include "logger.h"
#include "SoundBuffer.h"

namespace native {

SoundBuffer::SoundBuffer(AssetStorage* assets, const char* filename)
  : m_read_mode(ReadMode::ASSETS)
  , m_assets(assets)
  , m_filename(new char[128])
  , m_length(0)
  , m_data(nullptr)
  , m_error_code(0) {
  strcpy(m_filename, filename);
}

SoundBuffer::SoundBuffer(const char* filepath)
  : m_read_mode(ReadMode::FILESYSTEM)
  , m_assets(nullptr)
  , m_filename(new char[128])
  , m_length(0)
  , m_data(nullptr)
  , m_error_code(0) {
  strcpy(m_filename, filepath);
}

SoundBuffer::~SoundBuffer() {
  m_assets = nullptr;
  delete [] m_filename;  m_filename = nullptr;
  unload();
}

const char* SoundBuffer::getFilename() const { return m_filename; }

const char* SoundBuffer::getName() const {
  if (m_filename != nullptr) {
    std::string lower = m_filename;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return basename(lower.c_str());
  }
  return nullptr;
}

uint8_t* const SoundBuffer::getData() const { return m_data; }
off_t SoundBuffer::getLength() const { return m_length; }

bool SoundBuffer::load() {
  m_data = loadSound();
  if (m_data == nullptr) {
    ERR("Internal error during loading sound! Code: %i", m_error_code);
    return false;
  }
  return true;
}

void SoundBuffer::unload() {
  delete [] m_data;  m_data = nullptr;
  m_length = 0;
}

// ----------------------------------------------------------------------------
WAVSound::WAVSound(AssetStorage* assets, const char* filename)
  : SoundBuffer(assets, filename) {
}

WAVSound::WAVSound(const char* filepath)
  : SoundBuffer(filepath) {
}

WAVSound::~WAVSound() {
}

uint8_t* WAVSound::loadSound() {
  WAVHeader* header = new WAVHeader();
  uint8_t* data = nullptr;
  FILE* file_descriptor = nullptr;
  int error_code = 0;

  switch (m_read_mode) {
    case ReadMode::ASSETS:
      if (!m_assets->open(m_filename)) { error_code = 1; goto ERROR_SOUND; }
      m_length = m_assets->length() - sizeof(WAVHeader);
      data = new (std::nothrow) uint8_t[m_length];
      if (data == nullptr) { error_code = 2; goto ERROR_SOUND; }
      m_assets->read(header, sizeof(WAVHeader));
      if (!m_assets->read(data, m_length)) { error_code = 3; goto ERROR_SOUND; }
      m_assets->close();
      break;
    case ReadMode::FILESYSTEM:
      file_descriptor = std::fopen(m_filename, "rb");
      if (file_descriptor == nullptr) { error_code = 4; goto ERROR_SOUND; }
      fseek(file_descriptor, 0, SEEK_END);
      m_length = ftell(file_descriptor) - sizeof(WAVHeader);
      rewind(file_descriptor);
      data = new (std::nothrow) uint8_t(m_length);
      if (data == nullptr) { error_code = 5; goto ERROR_SOUND; }
      std::fread(header, 1, sizeof(WAVHeader), file_descriptor);
      if (m_length != std::fread(data, 1, m_length, file_descriptor)) { error_code = 6; goto ERROR_SOUND; }
      std::fclose(file_descriptor);
      break;
  }
  delete header;
  return data;

  ERROR_SOUND:
    m_error_code = 2020 + error_code;
    ERR("Error while reading raw sound: %i", m_error_code);
    switch (m_read_mode) {
      case ReadMode::ASSETS:
        m_assets->close();
        break;
      case ReadMode::FILESYSTEM:
        std::fclose(file_descriptor);
        break;
    }
    if (data != nullptr) {
      delete [] data;  data = nullptr;
    }
    delete header;
    return nullptr;
}

}
