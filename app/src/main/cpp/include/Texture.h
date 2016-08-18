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

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <libgen.h>
#include <GLES/gl.h>
#include <png.h>

#include "AssetStorage.h"

namespace native {

enum class ImageCode : int {
  none = 1000, png  = 1020
};

static const char* toString(ImageCode code) {
  switch (code) {
    default:
    case ImageCode::png:
      return "png";
  }
}

class Texture {
public:
  Texture(AssetStorage* assets, const char* filename);
  Texture(const char* filepath);
  virtual ~Texture();

  GLuint getID() const;
  GLint getFormat() const;
  int32_t getWidth() const;
  int32_t getHeight() const;
  const char* getFilename() const;
  const char* getName() const;
  int getErrorCode() const;

  virtual bool load();
  virtual void unload();
  virtual void apply() const;

protected:
  virtual const uint8_t* loadImage() = 0;

  enum class ReadMode : int {
    ASSETS = 0, FILESYSTEM = 1
  };

  ReadMode m_read_mode;
  AssetStorage* m_assets;
  char* m_filename;
  unsigned int m_data_size;
  GLuint m_id;
  GLint m_format;
  GLint m_type;
  uint32_t m_width;
  uint32_t m_height;
  int m_error_code;
};

// ----------------------------------------------------------------------------
class PNGTexture : public Texture {
public:
  PNGTexture(AssetStorage* assets, const char* filename);
  PNGTexture(const char* filepath);
  virtual ~PNGTexture();

protected:
  const uint8_t* loadImage() override final;

private:
  static void callback_read_assets(png_structp png, png_bytep data, png_size_t size);
  static void callback_read_file(png_structp png, png_bytep data, png_size_t size);
};

/**
 * Error codes (PNG):
 *
 * 102001 - assets->open() failed
 * 102002 - assets->read() header failed
 * 102003 - png_sig_cmp() failed, wrong header of PNG file
 * 102004 - png_create_read_struct() failed
 * 102005 - png_create_info_struct() failed
 * 102006 - setjmp(png_jmpbuf()) failed
 * 102007 - png_get_rowbytes() failed
 * 102008 - image_buffer allocation failed
 * 102009 - row_ptrs allocation failed
 * 102010 - fopen() failed
 * 102011 - fread() for header of PNG file failed
 */

}  // namespace native

#endif /* TEXTURE_H_ */
