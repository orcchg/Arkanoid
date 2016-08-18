/**
 * Copyright (c) 2015, Alov Maxim <alovmax@yandex.ru>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions
 * and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided with
 * the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <algorithm>
#include <cstdio>
#include <cstring>

#include "logger.h"
#include "Texture.h"


namespace native {

Texture::Texture(AssetStorage* assets, const char* filename)
  : m_read_mode(ReadMode::ASSETS)
  , m_assets(assets)
  , m_filename(new char[128])
  , m_data_size(0)
  , m_id(0)
  , m_format(0)
  , m_width(0)
  , m_height(0)
  , m_error_code(0) {
  strcpy(m_filename, filename);
}

Texture::Texture(const char* filepath)
  : m_read_mode(ReadMode::FILESYSTEM)
  , m_assets(nullptr)
  , m_filename(new char[128])
  , m_data_size(0)
  , m_id(0)
  , m_format(0)
  , m_width(0)
  , m_height(0)
  , m_error_code(0) {
  strcpy(m_filename, filepath);
}

Texture::~Texture() {
  m_assets = nullptr;
  delete [] m_filename;  m_filename = nullptr;
  unload();
}

GLuint Texture::getID() const { return m_id; }
GLint Texture::getFormat() const { return m_format; }
int32_t Texture::getWidth() const { return m_width; }
int32_t Texture::getHeight() const { return m_height; }
const char* Texture::getFilename() const { return m_filename; }
int Texture::getErrorCode() const { return m_error_code; }

const char* Texture::getName() const {
  if (m_filename != nullptr) {
    std::string lower = m_filename;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return basename(lower.c_str());
  }
  return nullptr;
}

bool Texture::load() {
  const uint8_t* image_buffer = loadImage();
  if (image_buffer == nullptr) {
    ERR("Internal error during loading texture! Code: %i", m_error_code);
    return false;
  }

  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
//  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, m_type, image_buffer);
  delete [] image_buffer;  image_buffer = nullptr;
  glBindTexture(GL_TEXTURE_2D, 0);

  GLenum glerror = glGetError();
  if (glerror != GL_NO_ERROR) {
    ERR("Error loading texture %s into OpenGL, gl error %zu, Details: fmt=%i, w=%zu h=%zu, type=%i",
        m_filename, glerror, m_format, m_width, m_height, m_type);
    unload();
    return false;
  }
  return true;
}

void Texture::unload() {
  glBindTexture(GL_TEXTURE_2D, 0);
  if (m_id != 0) {
    glDeleteTextures(1, &m_id);
    m_id = 0;
  }
  m_format = 0;
  m_width = 0;
  m_height = 0;
}

void Texture::apply() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_id);
}

// ----------------------------------------------------------------------------
PNGTexture::PNGTexture(AssetStorage* assets, const char* filename)
  : Texture(assets, filename) {
}

PNGTexture::PNGTexture(const char* filepath)
  : Texture(filepath) {
}

PNGTexture::~PNGTexture() {
}

const uint8_t* PNGTexture::loadImage() {
  FILE* file_descriptor = nullptr;
  png_byte header[8];
  png_structp png_ptr = nullptr;
  png_infop info_ptr = nullptr;
  png_byte* image_buffer = nullptr;
  png_bytep* row_ptrs = nullptr;
  png_int_32 row_size = 0;
  bool transparency = false;
  int error_code = 0;

  size_t header_size = sizeof(header);
  switch (m_read_mode) {
    case ReadMode::ASSETS:
      if (!m_assets->open(m_filename)) { error_code = 1; goto ERROR_PNG; }
      if (!m_assets->read(header, header_size)) { error_code = 2; goto ERROR_PNG; }
      break;
    case ReadMode::FILESYSTEM:
      file_descriptor = std::fopen(m_filename, "rb");
      if (file_descriptor == nullptr) { error_code = 10; goto ERROR_PNG; }
      if (header_size != std::fread(header, 1, header_size, file_descriptor)) { error_code = 11; goto ERROR_PNG; }
      break;
  }
  if (png_sig_cmp(header, 0, 8) != 0) { error_code = 3; goto ERROR_PNG; }

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (png_ptr == nullptr) { error_code = 4; goto ERROR_PNG; }
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == nullptr) { error_code = 5; goto ERROR_PNG; }

  switch (m_read_mode) {
    case ReadMode::ASSETS:
      png_set_read_fn(png_ptr, m_assets, callback_read_assets);
      break;
    case ReadMode::FILESYSTEM:
      png_set_read_fn(png_ptr, file_descriptor, callback_read_file);
      break;
  }
  {
    int jump_code = 0;
    if ( (jump_code = setjmp(png_jmpbuf(png_ptr))) != 0) {
      ERR("PNGTexture: setjump() returned code %i", jump_code);
      error_code = 6; goto ERROR_PNG;
    }
  }

  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);

  png_int_32 depth, color_type;
  png_uint_32 width, height;
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &depth, &color_type, nullptr, nullptr, nullptr);
  m_width = width;  m_height = height;

  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(png_ptr);
    transparency = true;
  }

  if (depth < 8) {
    png_set_packing(png_ptr);
  } else {
    png_set_strip_16(png_ptr);
  }

  switch (color_type) {
    case PNG_COLOR_TYPE_PALETTE:
      png_set_palette_to_rgb(png_ptr);
      m_format = transparency ? GL_RGBA : GL_RGB;
      break;
    case PNG_COLOR_TYPE_RGB:
      m_format = transparency ? GL_RGBA : GL_RGB;
      break;
    case PNG_COLOR_TYPE_RGBA:
      m_format = GL_RGBA;
      break;
    case PNG_COLOR_TYPE_GRAY:
      png_set_expand_gray_1_2_4_to_8(png_ptr);
      m_format = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
      break;
    case PNG_COLOR_TYPE_GA:
      png_set_expand_gray_1_2_4_to_8(png_ptr);
      m_format = GL_LUMINANCE_ALPHA;
      break;
  }
  m_type = GL_UNSIGNED_BYTE;
  png_read_update_info(png_ptr, info_ptr);

  row_size = png_get_rowbytes(png_ptr, info_ptr);
  if (row_size <= 0) { error_code = 7; goto ERROR_PNG; }
  image_buffer = new (std::nothrow) png_byte[row_size * height];
  if (image_buffer == nullptr) { error_code = 8; goto ERROR_PNG; }

  row_ptrs = new (std::nothrow) png_bytep[height];
  if (row_ptrs == nullptr) { error_code = 9; goto ERROR_PNG; }
  for (int32_t i = 0; i < height; ++i) {
    row_ptrs[height - (i + 1)] = image_buffer + i * row_size;
  }
  png_read_image(png_ptr, row_ptrs);

  switch (m_read_mode) {
    case ReadMode::ASSETS:
      m_assets->close();
      break;
    case ReadMode::FILESYSTEM:
      std::fclose(file_descriptor);
      break;
  }
  png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
  delete [] row_ptrs;
  return image_buffer;

  ERROR_PNG:
    m_error_code = static_cast<int>(ImageCode::png) * 100 + error_code;
    ERR("Error while reading PNG file: %s, code %i", m_filename, m_error_code);
    switch (m_read_mode) {
      case ReadMode::ASSETS:
        m_assets->close();
        break;
      case ReadMode::FILESYSTEM:
        std::fclose(file_descriptor);
        break;
    }
    delete [] image_buffer;  image_buffer = nullptr;
    delete [] row_ptrs;  row_ptrs = nullptr;
    if (png_ptr != nullptr) {
      png_infop* info_ptr_p = info_ptr != nullptr ? &info_ptr : nullptr;
      png_destroy_read_struct(&png_ptr, info_ptr_p, nullptr);
    }
    return nullptr;
}

void PNGTexture::callback_read_assets(png_structp io, png_bytep data, png_size_t size) {
  AssetStorage* assets = (AssetStorage*) png_get_io_ptr(io);
  if (!assets->read(data, size)) {
    assets->close();
    png_error(io, "Error while reading PNG file (from callback_read_assets()) !");
  }
}

void PNGTexture::callback_read_file(png_structp io, png_bytep data, png_size_t size) {
  FILE* file = (FILE*) png_get_io_ptr(io);
  if (size != std::fread(data, 1, size, file)) {
    std::fclose(file);
    png_error(io, "Error while reading PNG file (from callback_read_file()) !");
  }
}

}  // namespace native
