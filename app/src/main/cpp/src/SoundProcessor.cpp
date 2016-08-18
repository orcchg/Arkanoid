#include <sstream>

#include "Exceptions.h"
#include "logger.h"
#include "SoundProcessor.h"

namespace native {
namespace sound {

SoundProcessor::SoundProcessor(JavaVM* jvm)
  : m_jvm(jvm), m_jenv(nullptr)
  , master_object(nullptr)
  , m_error_code(0)
  , m_engine(nullptr)
  , m_mixer(nullptr)
  , m_interface(nullptr)
  , m_players(new SoundPlayer[playersCount])
  , m_queue_size(0)
  , m_selected_player(0)
  , m_impacted_block(game::Block::NONE)
  , m_prize(game::Prize::NONE)
  , m_ball_effect(game::BallEffect::NONE) {

  DBG("enter SoundProcessor ctor");
  if (!init()) {
    std::ostringstream oss;
    oss << "Sound processor exception, error code: " << m_error_code;
    throw SoundProcessorException(oss.str().c_str());
  }

  m_load_resources_received.store(false);
  m_lost_ball_received.store(false);
  m_bite_impact_received.store(false);
  m_block_impact_received.store(false);
  m_wall_impact_received.store(false);
  m_level_finished_received.store(false);
  m_explosion_received.store(false);
  m_prize_caught_received.store(false);
  m_laser_beam_visibility_received.store(false);
  m_laser_block_impact_received.store(false);
  m_laser_pulse_received.store(false);
  m_ball_effect_received.store(false);
  DBG("exit SoundProcessor ctor");
}

SoundProcessor::~SoundProcessor() {
  DBG("enter SoundProcessor ~dtor");
  m_jvm = nullptr;  m_jenv = nullptr;  master_object = nullptr;
  destroy();
  m_resources = nullptr;
  DBG("exit SoundProcessor ~dtor");
}

/* Callbacks group */
// ----------------------------------------------------------------------------
void SoundProcessor::callback_loadResources(bool /* dummy */) {
  std::unique_lock<std::mutex> lock(m_load_resources_mutex);
  m_load_resources_received.store(true);
  interrupt();
}

void SoundProcessor::callback_lostBall(float is_lost) {
  std::unique_lock<std::mutex> lock(m_lost_ball_mutex);
  m_lost_ball_received.store(true);
  interrupt();
}

void SoundProcessor::callback_biteImpact(bool /* dummy */) {
  std::unique_lock<std::mutex> lock(m_bite_impact_mutex);
  m_bite_impact_received.store(true);
  interrupt();
}

void SoundProcessor::callback_blockImpact(game::RowCol block) {
  std::unique_lock<std::mutex> lock(m_block_impact_mutex);
  m_block_impact_received.store(true);
  m_impacted_block = block.block;
  interrupt();
}

void SoundProcessor::callback_wallImpact(bool /* dummy */) {
  std::unique_lock<std::mutex> lock(m_wall_impact_mutex);
  m_wall_impact_received.store(true);
  interrupt();
}

void SoundProcessor::callback_levelFinished(bool is_finished) {
  std::unique_lock<std::mutex> lock(m_level_finished_mutex);
  m_level_finished_received.store(true);
  interrupt();
}

void SoundProcessor::callback_explosion(game::ExplosionPackage package) {
  std::unique_lock<std::mutex> lock(m_explosion_mutex);
  m_explosion_received.store(true);
  interrupt();
}

void SoundProcessor::callback_prizeCaught(game::PrizePackage package) {
  std::unique_lock<std::mutex> lock(m_prize_caught_mutex);
  m_prize_caught_received.store(true);
  m_prize = package.getPrize();
  interrupt();
}

void SoundProcessor::callback_laserBeamVisibility(bool is_visible) {
  std::unique_lock<std::mutex> lock(m_laser_beam_visibility_mutex);
  m_laser_beam_visibility_received.store(true);
  interrupt();
}

void SoundProcessor::callback_laserBlockImpact(bool /* dummy */) {
  std::unique_lock<std::mutex> lock(m_laser_block_impact_mutex);
  m_laser_block_impact_received.store(true);
  interrupt();
}

void SoundProcessor::callback_laserPulse(bool /* dummy */) {
  std::unique_lock<std::mutex> lock(m_laser_pulse_mutex);
  m_laser_pulse_received.store(true);
  interrupt();
}

void SoundProcessor::callback_ballEffect(game::BallEffect effect) {
  std::unique_lock<std::mutex> lock(m_ball_effect_mutex);
  m_ball_effect_received.store(true);
  m_ball_effect = effect;
  interrupt();
}

// ----------------------------------------------
void SoundProcessor::setResourcesPtr(game::Resources* resources) {
  m_resources = resources;
}

/* *** Private methods *** */
/* JNIEnvironment group */
// ----------------------------------------------------------------------------
void SoundProcessor::attachToJVM() {
  std::unique_lock<std::mutex> lock(m_jnienvironment_mutex);
  auto result = m_jvm->AttachCurrentThread(&m_jenv, nullptr /* thread args */);
  if (result != JNI_OK) {
    ERR("SoundProcessor thread was not attached to JVM !");
    throw JNI_exception();
  }
}

void SoundProcessor::detachFromJVM() {
  std::unique_lock<std::mutex> lock(m_jnienvironment_mutex);
  m_jvm->DetachCurrentThread();
}

/* ActiveObject group */
// ----------------------------------------------------------------------------
void SoundProcessor::onStart() {
  DBG("SoundProcessor onStart");
}

void SoundProcessor::onStop() {
  DBG("SoundProcessor onStop");
}

bool SoundProcessor::checkForWakeUp() {
  return m_load_resources_received.load() ||
      m_lost_ball_received.load() ||
      m_bite_impact_received.load() ||
      m_block_impact_received.load() ||
      m_wall_impact_received.load() ||
      m_level_finished_received.load() ||
      m_explosion_received.load() ||
      m_prize_caught_received.load() ||
      m_laser_beam_visibility_received.load() ||
      m_laser_block_impact_received.load() ||
      m_laser_pulse_received.load() ||
      m_ball_effect_received.load();
}

void SoundProcessor::eventHandler() {
  if (m_load_resources_received.load()) {
    m_load_resources_received.store(false);
    process_loadResources();
  }
  if (m_explosion_received.load()) {
    m_explosion_received.store(false);
    process_explosion();
  }
  if (m_prize_caught_received.load()) {
    m_prize_caught_received.store(false);
    process_prizeCaught();
  }
  if (m_bite_impact_received.load()) {
    m_bite_impact_received.store(false);
    process_biteImpact();
  }
  if (m_block_impact_received.load()) {
    m_block_impact_received.store(false);
    process_blockImpact();
  }
  if (m_wall_impact_received.load()) {
    m_wall_impact_received.store(false);
    process_wallImpact();
  }
  if (m_lost_ball_received.load()) {
    m_lost_ball_received.store(false);
    process_lostBall();
  }
  if (m_level_finished_received.load()) {
    m_level_finished_received.store(false);
    process_levelFinished();
  }
  if (m_laser_beam_visibility_received.load()) {
    m_laser_beam_visibility_received.store(false);
    process_laserBeamVisibility();
  }
  if (m_laser_block_impact_received.load()) {
    m_laser_block_impact_received.store(false);
    process_laserBlockImpact();
  }
  if (m_laser_pulse_received.load()) {
    m_laser_pulse_received.store(false);
    process_laserPulse();
  }
  if (m_ball_effect_received.load()) {
    m_ball_effect_received.store(false);
    process_ballEffect();
  }
}

/* Processors group */
// ----------------------------------------------------------------------------
void SoundProcessor::process_loadResources() {
  std::unique_lock<std::mutex> lock(m_load_resources_mutex);
  if (m_resources != nullptr) {
    for (auto it = m_resources->beginSound(); it != m_resources->endSound(); ++it) {
      DBG("Loading sound resources: %s %p", it->first.c_str(), it->second);
      if (!it->second->load()) {
        // notify Java layer about internal problem
        m_jenv->CallVoidMethod(master_object, fireJavaEvent_errorSoundLoad_id);
      }
    }
  } else {
    ERR("Resources pointer was not set !");
  }
}

void SoundProcessor::process_lostBall() {
  std::unique_lock<std::mutex> lock(m_lost_ball_mutex);
  auto sound = m_resources->getRandomSound("lose_");
  playSound(sound);
}

void SoundProcessor::process_biteImpact() {
  std::unique_lock<std::mutex> lock(m_bite_impact_mutex);
  auto sound = m_resources->getRandomSound("bite_");
  playSound(sound);
}

void SoundProcessor::process_blockImpact() {
  std::unique_lock<std::mutex> lock(m_block_impact_mutex);
  std::string sound_prefix = "";

  switch (m_impacted_block) {
    case game::Block::ALUMINIUM:
    case game::Block::BRICK:
    case game::Block::CLAY:
    case game::Block::JELLY:
    case game::Block::ROLLING:
    case game::Block::SIMPLE:
      sound_prefix = "block_";
      break;
    case game::Block::FOG:
      sound_prefix = "fog_";
      break;
    case game::Block::GLASS:
    case game::Block::GLASS_1:
      sound_prefix = "glass_";
      break;
    case game::Block::ELECTRO:
    case game::Block::KNOCK_VERTICAL:
    case game::Block::KNOCK_HORIZONTAL:
      sound_prefix = "explode_";
      break;
    case game::Block::ARTIFICAL:
      sound_prefix = "magic_";
      break;
    case game::Block::MAGIC:
      // TODO: magic explosion
      break;
    case game::Block::IRON:
    case game::Block::STEEL:
    case game::Block::PLUMBUM:
      sound_prefix = "iron_";
      break;
    case game::Block::HYPER:
    case game::Block::ORIGIN:
    case game::Block::NETWORK:
      sound_prefix = "hyper_";
      break;
    case game::Block::ULTRA:
    case game::Block::ULTRA_4:
    case game::Block::ULTRA_3:
    case game::Block::ULTRA_2:
    case game::Block::ULTRA_1:
      sound_prefix = "ultra_";
      break;
    case game::Block::TITAN:
    case game::Block::INVUL:
    case game::Block::EXTRA:
      sound_prefix = "invul_";
      break;
    case game::Block::QUICK:
    case game::Block::QUICK_2:
      sound_prefix = "block_";
      break;
    case game::Block::QUICK_1:
      // TODO: quick impact
      break;
    case game::Block::WATER:
    case game::Block::YOGURT:
    case game::Block::YOGURT_1:
      sound_prefix = "water_";
      break;
    case game::Block::ZYGOTE:
    case game::Block::ZYGOTE_1:
    case game::Block::ZYGOTE_SPAWN:
      sound_prefix = "zygote_";
      break;
    case game::Block::DESTROY:
    case game::Block::MIDAS:
      sound_prefix = "destroy_";
      break;
    case game::Block::NONE:
    default:
      return;
  }

  auto sound = m_resources->getRandomSound(sound_prefix);
  playSound(sound);
}

void SoundProcessor::process_wallImpact() {
//  std::unique_lock<std::mutex> lock(m_wall_impact_mutex);
  // no-op
}

void SoundProcessor::process_levelFinished() {
  std::unique_lock<std::mutex> lock(m_level_finished_mutex);
  auto sound = m_resources->getRandomSound("win_");
  playSound(sound);
}

void SoundProcessor::process_explosion() {
//  std::unique_lock<std::mutex> lock(m_explosion_mutex);
  // no-op
}

void SoundProcessor::process_prizeCaught() {
  std::unique_lock<std::mutex> lock(m_prize_caught_mutex);
  std::string sound_prefix = "";

  switch (m_prize) {
    case game::Prize::DESTROY:
      sound_prefix = "skull_";
      break;
    case game::Prize::HYPER:
      sound_prefix = "hyper_";
      break;
    case game::Prize::VITALITY:
      sound_prefix = "vitality_";
      break;
    case game::Prize::WIN:
      sound_prefix = "win_";
      break;
    default:
      sound_prefix = "prize_";
      break;
  }
  auto sound = m_resources->getRandomSound(sound_prefix);
  playSound(sound);
}

void SoundProcessor::process_laserBeamVisibility() {
//  std::unique_lock<std::mutex> lock(m_laser_beam_visibility_mutex);
  // no-op
}

void SoundProcessor::process_laserBlockImpact() {
  std::unique_lock<std::mutex> lock(m_laser_block_impact_mutex);
  // no-op
}

void SoundProcessor::process_laserPulse() {
  std::unique_lock<std::mutex> lock(m_laser_pulse_mutex);
  auto sound = m_resources->getRandomSound("laser_");
  playSound(sound);
}

void SoundProcessor::process_ballEffect() {
  std::unique_lock<std::mutex> lock(m_ball_effect_mutex);
  std::string sound_prefix = "";

  // TODO: more accurate sounds
  switch (m_ball_effect) {
    case game::BallEffect::EASY:
    case game::BallEffect::EASY_T:
    case game::BallEffect::EXPLODE:
    case game::BallEffect::JUMP:
    case game::BallEffect::PIERCE:
      sound_prefix = "explode_";
      break;
    case game::BallEffect::UPGRADE:
      sound_prefix = "upgrade_";
      break;
    case game::BallEffect::DEGRADE:
      sound_prefix = "degrade_";
      break;
    default:
      return;  // no sound to play
  }
  auto sound = m_resources->getRandomSound(sound_prefix);
  playSound(sound);
}

/* CoreFunc group */
// ----------------------------------------------------------------------------
bool SoundProcessor::init() {
  const SLInterfaceID ids[1] {SL_IID_ENGINE};
  const SLboolean required[1] {SL_BOOLEAN_TRUE};
  const SLInterfaceID mix_ids[] {};
  const SLboolean mix_required[] {};
  bool queue_init_result = true;
  int error_code = 0;

  SLresult result = slCreateEngine(&m_engine, 0, nullptr, 1, ids, required);
  if (result != SL_RESULT_SUCCESS) { error_code = 1; goto ERROR_SOUND; }
  result = (*m_engine)->Realize(m_engine, SL_BOOLEAN_FALSE);
  if (result != SL_RESULT_SUCCESS) { error_code = 2; goto ERROR_SOUND; }
  result = (*m_engine)->GetInterface(m_engine, SL_IID_ENGINE, &m_interface);
  if (result != SL_RESULT_SUCCESS) { error_code = 3; goto ERROR_SOUND; }
  result = (*m_interface)->CreateOutputMix(m_interface, &m_mixer, 0, mix_ids, mix_required);
  if (result != SL_RESULT_SUCCESS) { error_code = 4; goto ERROR_SOUND; }
  result = (*m_mixer)->Realize(m_mixer, SL_BOOLEAN_FALSE);
  if (result != SL_RESULT_SUCCESS) { error_code = 5; goto ERROR_SOUND; }

  for (int i = 0; i < SoundProcessor::playersCount; ++i) {
    queue_init_result &= initPlayerQueue(i);
  }
  return queue_init_result;

  ERROR_SOUND:
    m_error_code = 2000 + error_code;
    ERR("Error while initializing sound processor: %i", m_error_code);
    destroy();
    return false;
}

bool SoundProcessor::initPlayerQueue(int player_id) {
  SLDataLocator_AndroidSimpleBufferQueue data_locator_in {
    SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
    SoundProcessor::queueMaxSize  // no more than 'queueMaxSize' sound buffer in queue at any moment
  };

  SLDataFormat_PCM data_format {
    SL_DATAFORMAT_PCM,
    1 /* mono sound */,
    SL_SAMPLINGRATE_44_1,
    SL_PCMSAMPLEFORMAT_FIXED_16,
    SL_PCMSAMPLEFORMAT_FIXED_16,
    SL_SPEAKER_FRONT_CENTER,
    SL_BYTEORDER_LITTLEENDIAN
  };

  SLDataSource data_source {
    &data_locator_in,
    &data_format
  };

  SLDataLocator_OutputMix data_locator_out {
    SL_DATALOCATOR_OUTPUTMIX,
    m_mixer
  };

  SLDataSink data_sink {
    &data_locator_out,
    nullptr
  };

  const SLInterfaceID player_ids[2] { SL_IID_PLAY, SL_IID_BUFFERQUEUE };
  const SLboolean player_required[2] { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

  int error_code = 0;
  SLresult result = (*m_interface)->CreateAudioPlayer(m_interface, &m_players[player_id].m_player, &data_source, &data_sink, 2, player_ids, player_required);
  if (result != SL_RESULT_SUCCESS) { error_code = 6; goto ERROR_PLAYER; }
  result = (*m_players[player_id].m_player)->Realize(m_players[player_id].m_player, SL_BOOLEAN_FALSE);
  if (result != SL_RESULT_SUCCESS) { error_code = 7; goto ERROR_PLAYER; }
  result = (*m_players[player_id].m_player)->GetInterface(m_players[player_id].m_player, SL_IID_PLAY, &m_players[player_id].m_player_interface);
  if (result != SL_RESULT_SUCCESS) { error_code = 8; goto ERROR_PLAYER; }
  result = (*m_players[player_id].m_player)->GetInterface(m_players[player_id].m_player, SL_IID_BUFFERQUEUE, &m_players[player_id].m_player_queue);
  if (result != SL_RESULT_SUCCESS) { error_code = 9; goto ERROR_PLAYER; }
  result = (*m_players[player_id].m_player_interface)->SetPlayState(m_players[player_id].m_player_interface, SL_PLAYSTATE_PLAYING);
  if (result != SL_RESULT_SUCCESS) { error_code = 10; goto ERROR_PLAYER; }
  return true;

  ERROR_PLAYER:
    m_error_code = 2000 + error_code;
    ERR("Error while initializing sound buffers queue: %i", m_error_code);
    return false;
}

bool SoundProcessor::playSound(const SoundBuffer* sound) {
  if (m_selected_player >= SoundProcessor::playersCount) {
    m_selected_player = 0;
  }

  int error_code = 0;
  SLuint32 player_state = 0;
  (*m_players[m_selected_player].m_player)->GetState(m_players[m_selected_player].m_player, &player_state);

  if (player_state == SL_OBJECT_STATE_REALIZED) {
    SLresult result = SL_RESULT_SUCCESS;
    if (m_queue_size >= SoundProcessor::queueMaxSize) {
      // stop all sounds in queue before playing a new one
      result = (*m_players[m_selected_player].m_player_queue)->Clear(m_players[m_selected_player].m_player_queue);
      if (result != SL_RESULT_SUCCESS) { error_code = 11; goto ERROR_PLAY; }
      m_queue_size = 0;
    }
    int16_t* buffer = reinterpret_cast<int16_t*>(sound->getData());
    off_t length = sound->getLength();
    result = (*m_players[m_selected_player].m_player_queue)->Enqueue(m_players[m_selected_player].m_player_queue, buffer, length);
    if (result != SL_RESULT_SUCCESS) { error_code = 12; goto ERROR_PLAY; }
    ++m_queue_size;
    ++m_selected_player;
  }
  return true;

  ERROR_PLAY:
    m_error_code = 2000 + error_code;
    ERR("Error while playing sound %s, code: %i", sound->getName(), m_error_code);
    return false;
}

void SoundProcessor::destroy() {
  if (m_players != nullptr) {
    delete [] m_players;
    m_players = nullptr;
  }
  if (m_mixer != nullptr) {
    (*m_mixer)->Destroy(m_mixer);
    m_mixer = nullptr;
  }
  if (m_engine != nullptr) {
    (*m_engine)->Destroy(m_engine);
    m_engine = nullptr;
  }
}

}
}
