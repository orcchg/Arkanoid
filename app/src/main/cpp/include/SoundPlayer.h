#ifndef __ARKANOID_SOUNDPLAYER__H__
#define __ARKANOID_SOUNDPLAYER__H__

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

namespace native {
namespace sound {

class SoundProcessor;

class SoundPlayer {
  friend class SoundProcessor;
public:
  SoundPlayer();
  virtual ~SoundPlayer();

private:
  SLObjectItf m_player;
  SLPlayItf m_player_interface;
  SLBufferQueueItf m_player_queue;
};

}  // namespace sound
}  // namespace native

#endif /* __ARKANOID_SOUNDPLAYER__H__ */
