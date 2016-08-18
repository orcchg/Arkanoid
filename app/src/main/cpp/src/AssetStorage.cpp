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

#include <cstring>
#include "AssetStorage.h"
#include "Exceptions.h"
#include "logger.h"


AssetStorage::AssetStorage(JNIEnv* jenv, const jobject& assetManager)
  : m_internal_file_storage(new char[256])
  , m_asset_filename(nullptr)
  , m_length(-1)
  , m_manager(AAssetManager_fromJava(jenv, assetManager))
  , m_asset(nullptr) {
}

AssetStorage::~AssetStorage() {
  delete [] m_internal_file_storage;
}

const char* AssetStorage::getName() const {
  return m_asset_filename;
}

const char* AssetStorage::getInternalFileStorage() const {
  return m_internal_file_storage;
}

void AssetStorage::setInternalFileStorage(const char* path) {
  strcpy(m_internal_file_storage, path);
}

bool AssetStorage::open(const char* asset_filename) {
  m_asset = AAssetManager_open(m_manager, asset_filename, AASSET_MODE_UNKNOWN);
  if (m_asset == nullptr) {
    ERR("Failed to open asset from file: %s!", m_asset_filename);
    return false;
  }
  m_asset_filename = asset_filename;
  m_length = AAsset_getLength(m_asset);
  return true;
}

void AssetStorage::close() {
  if (m_asset != nullptr) {
    AAsset_close(m_asset);
    m_asset = nullptr;
  }
  m_asset_filename = nullptr;
  m_length = -1;
}

bool AssetStorage::read(void* buffer) {
  if (buffer == nullptr) {
    ERR("Buffer not allocated for asset!");
    return false;
  }
  int32_t read_count = AAsset_read(m_asset, buffer, m_length);
  if (read_count != m_length) {
    ERR("Error during reading asset from file: %s!", m_asset_filename);
    return false;
  }
  return true;
}

bool AssetStorage::read(void* buffer, size_t size) {
  if (buffer == nullptr) {
    ERR("Buffer not allocated for asset!");
    return false;
  }
  int32_t read_count = AAsset_read(m_asset, buffer, size);
  if (read_count != size) {
    ERR("Error during reading asset from file: %s!", m_asset_filename);
    return false;
  }
  return true;
}
