#ifndef __ARKANOID_RESOURCES__H__
#define __ARKANOID_RESOURCES__H__

/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_orcchg_arkanoid_surface_NativeResources */

#ifndef _Included_com_orcchg_arkanoid_surface_NativeResources
#define _Included_com_orcchg_arkanoid_surface_NativeResources
#ifdef __cplusplus
extern "C" {
#endif

/* Init */
// ----------------------------------------------------------------------------
/*
 * Class:     com_orcchg_arkanoid_surface_NativeResources
 * Method:    init
 * Signature: (Landroid/content/res/AssetManager;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_orcchg_arkanoid_surface_NativeResources_init
  (JNIEnv *, jobject, jobject, jstring);

/*
 * Class:     com_orcchg_arkanoid_surface_NativeResources
 * Method:    readTexture
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_orcchg_arkanoid_surface_NativeResources_readTexture
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_orcchg_arkanoid_surface_NativeResources
 * Method:    readSound
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_orcchg_arkanoid_surface_NativeResources_readSound
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_orcchg_arkanoid_surface_NativeResources
 * Method:    release
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_orcchg_arkanoid_surface_NativeResources_release
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif

/* Core */
// ----------------------------------------------------------------------------
#include <memory>
#include <string>
#include <unordered_map>
#include <cstdlib>

#include "Level.h"
#include "Prize.h"
#include "SoundBuffer.h"
#include "Texture.h"

namespace game {

class Resources : public std::enable_shared_from_this<Resources> {
public:
  typedef std::shared_ptr<Resources> Ptr;

  Resources(JNIEnv* jenv, jobject assets, jstring internalFileStorage_Java);
  ~Resources() noexcept;

  /** @defgroup Texture Access texture resources.
   * @{
   */
  typedef std::unordered_map<std::string, native::Texture*>::iterator tex_iterator;
  typedef std::unordered_map<std::string, native::Texture*>::const_iterator const_tex_iterator;

  bool readTexture(jstring filename);
  const native::Texture* const getTexture(const std::string& name) const;
  const native::Texture* const getRandomTexture(const std::string& prefix) const;
  const native::Texture* const getPrizeTexture(const Prize& prize) const;

  tex_iterator beginTexture();
  tex_iterator endTexture();
  const_tex_iterator cbeginTexture() const;
  const_tex_iterator cendTexture() const;
  /** @} */  // end of Texture group

  /** @defgroup Sound Access sound resources.
   * @{
   */
  typedef std::unordered_map<std::string, native::SoundBuffer*>::iterator sound_iterator;
  typedef std::unordered_map<std::string, native::SoundBuffer*>::const_iterator const_sound_iterator;

  bool readSound(jstring filename);
  const native::SoundBuffer* const getSound(const std::string& name) const;
  const native::SoundBuffer* const getRandomSound(const std::string& prefix) const;

  sound_iterator beginSound();
  sound_iterator endSound();
  const_sound_iterator cbeginSound() const;
  const_sound_iterator cendSound() const;
  /** @} */  // end of Sound group

  Ptr getSharedPtr();

private:
  JNIEnv* m_jenv;
  AssetStorage* m_assets;
  std::unordered_map<std::string, native::Texture*> m_textures;
  std::unordered_map<std::string, native::SoundBuffer*> m_sounds;
};

}

#endif  // __ARKANOID_RESOURCES__H__
