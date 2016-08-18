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

#ifndef SURFACE3D_ASSETSTORAGE_H_
#define SURFACE3D_ASSETSTORAGE_H_

#include <memory>
#include <jni.h>
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"

class AssetStorage {
public:
  typedef std::shared_ptr<AssetStorage> Ptr;

  AssetStorage(JNIEnv* jenv, const jobject& assetManager);
  virtual ~AssetStorage();

  const char* getName() const;
  const char* getInternalFileStorage() const;
  void setInternalFileStorage(const char* path);

  bool open(const char* asset_filename);
  void close();
  bool read(void* buffer);
  bool read(void* buffer, size_t size);
  inline off_t length() const {
    return m_length;
  }

private:
  char* m_internal_file_storage;
  const char* m_asset_filename;
  off_t m_length;
  AAssetManager* m_manager;
  AAsset* m_asset;
};

#endif /* SURFACE3D_ASSETSTORAGE_H_ */
