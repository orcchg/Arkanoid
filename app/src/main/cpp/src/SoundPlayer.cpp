#include "SoundPlayer.h"

namespace native {
namespace sound {

SoundPlayer::SoundPlayer() {
}

SoundPlayer::~SoundPlayer() {
  if (m_player != nullptr) {
    (*m_player)->Destroy(m_player);
    m_player = nullptr;
    m_player_interface = nullptr;
    m_player_queue = nullptr;
  }
}

}
}
