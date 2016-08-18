#ifndef __ARKANOID_SOUND_PROCESSOR__H__
#define __ARKANOID_SOUND_PROCESSOR__H__

#include <atomic>
#include <memory>
#include <mutex>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "ActiveObject.h"
#include "Ball.h"
#include "Block.h"
#include "Event.h"
#include "EventListener.h"
#include "ExplosionPackage.h"
#include "Prize.h"
#include "PrizePackage.h"
#include "Resources.h"
#include "RowCol.h"
#include "SoundPlayer.h"

namespace native {
namespace sound {

/// @class SoundProcessor SoundProcessor.h "include/SoundProcessor.h"
/// @brief Standalone thread to play sounds from sound buffers' queue.
/// http://habrahabr.ru/post/176933/
class SoundProcessor : public ActiveObject {
public:
  typedef SoundProcessor* Ptr;

  SoundProcessor(JavaVM* jvm);
  virtual ~SoundProcessor() noexcept;

  /** @defgroup Callbacks These methods are responses of incoming events
   *  which SoundProcessor subscribed on.
   *  @{
   */
  /// @brief Called when load resources requested.
  void callback_loadResources(bool /* dummy */);
  /// @brief Called when ball has been lost.
  void callback_lostBall(float is_lost);
  /// @brief Called when bite has been impacted.
  void callback_biteImpact(bool /* dummy */);
  /// @brief Called when block has been impacted.
  void callback_blockImpact(game::RowCol block);
  /// @brief Called when wall has been impacted.
  void callback_wallImpact(bool /* dummy */);
  /// @brief Called when level has been successfully finished.
  void callback_levelFinished(bool is_finished);
  /// @brief Called when requested to draw particle system explosion.
  void callback_explosion(game::ExplosionPackage package);
  /// @brief Called when prize has been caught.
  void callback_prizeCaught(game::PrizePackage package);
  /// @brief Called when laser beam changed visibility.
  void callback_laserBeamVisibility(bool is_visible);
  /// @brief Called when laser beam impacts block.
  void callback_laserBlockImpact(bool /* dummy */);
  /// @brief Called when laser pulse has emerged.
  void callback_laserPulse(bool /* dummy */);
  /// @brief Called when ball effect has occurred.
  void callback_ballEffect(game::BallEffect effect);
  /** @} */  // end of Callbacks group

  /** @defgroup Resources Bind with external resources.
   * @{
   */
  /// @brief Sets the pointer to external resources.
  void setResourcesPtr(game::Resources* resources);
  /** @} */  // end of Resources group

// ----------------------------------------------
/* Private member-functions */
private:
  /** @defgroup JNIEnvironment Native glue between core and GUI.
   * @{
   */
  /// @brief Attaches this thread to an existing JVM.
  void attachToJVM();
  /// @brief Detaches this thread from an existing JVM it had been
  /// previously attached.
  void detachFromJVM();
  /** @} */  // end of JNIEnvironment group

public:
  /** @addtogroup JNIEnvironment
   * @{
   */
  inline void setMasterObject(jobject object) { master_object = object; }
  inline void setOnErrorSoundLoadMethodID(jmethodID id) { fireJavaEvent_errorSoundLoad_id = id; }
  /** @} */  // end of JNIEnvironment group

// ----------------------------------------------
/* Public data-members */
public:
  /** @defgroup Event Outcoming events and listeners for incoming events.
   * @{
   */
  /// @brief Listens for load resources request.
  EventListener<bool> load_resources_listener;
  /// @brief Listens for event which occurs when ball has been lost.
  EventListener<bool> lost_ball_listener;
  /// @brief Listens for event which occurs when bite has been impacted.
  EventListener<bool> bite_impact_listener;
  /// @brief Listens for event which occurs when block has been impacted.
  EventListener<game::RowCol> block_impact_listener;
  /// @brief Listens for event which occurs when wall has been impacted.
  EventListener<bool> wall_impact_listener;
  /// @brief Listens for event which occurs when level has been successfully finished.
  EventListener<bool> level_finished_listener;
  /// @brief Listens for event which occurs when particle system explosion has been requested.
  EventListener<game::ExplosionPackage> explosion_listener;
  /// @brief Listens for event which occurs when prize has been caught.
  EventListener<game::PrizePackage> prize_caught_listener;
  /// @brief Listens for laser beam visibility.
  EventListener<bool> laser_beam_visibility_listener;
  /// @brief Listens for laser block impact.
  EventListener<bool> laser_block_impact_listener;
  /// @brief Listens for laser pulse has emerged.
  EventListener<bool> laser_pulse_listener;
  /// @brief Listens for ball effect occurred.
  EventListener<game::BallEffect> ball_effect_listener;
  /** @} */  // end of Event group

// ----------------------------------------------
/* Private data-members */
private:
  /** @addtogroup JNIEnvironment
   * @{
   */
  JavaVM* m_jvm;  //!< Pointer to Java Virtual Machine in current session.
  JNIEnv* m_jenv;  //!< Pointer to environment local within this thread.
  jobject master_object;
  jmethodID fireJavaEvent_errorSoundLoad_id;
  /** @} */  // end of JNIEnvironment group

  /** @defgroup Core Core data-structures for sound playback.
   * @{
   */
  int m_error_code;
  SLObjectItf m_engine;
  SLObjectItf m_mixer;
  SLEngineItf m_interface;

  SoundPlayer* m_players;
  int m_queue_size;
  int m_selected_player;

  constexpr static int queueMaxSize = 1;
  constexpr static int playersCount = 8;
  /** @} */  // end of Core group

  /** @defgroup LogicData Game Logic related data members.
   * @{
   */
  game::Block m_impacted_block;  //!< Last received impacted block.
  game::Prize m_prize;  //!< Last received prize.
  game::BallEffect m_ball_effect;  //!< Last received ball effect.
  /** @} */  // end of LogicData group

  /** @defgroup Mutex Thread-safety variables
   * @{
   */
  std::mutex m_jnienvironment_mutex;  //!< Sentinel for thread attach to JVM.
  std::mutex m_load_resources_mutex;  //!< Sentinel for load resources.
  std::mutex m_lost_ball_mutex;  //!< Sentinel for lost ball flag.
  std::mutex m_bite_impact_mutex;
  std::mutex m_block_impact_mutex;  //!< Sentinel for block impact event.
  std::mutex m_wall_impact_mutex;
  std::mutex m_level_finished_mutex;  //!< Sentinel for level has been successfully finished.
  std::mutex m_explosion_mutex;  //!< Sentinel for particle system explosion.
  std::mutex m_prize_caught_mutex;  //!< Sentinel for prize has been caught.
  std::mutex m_laser_beam_visibility_mutex;
  std::mutex m_laser_block_impact_mutex;
  std::mutex m_laser_pulse_mutex;
  std::mutex m_ball_effect_mutex;
  std::atomic_bool m_load_resources_received;  //!< Load resources requested.
  std::atomic_bool m_lost_ball_received;  //!< Ball has been lost received.
  std::atomic_bool m_bite_impact_received;
  std::atomic_bool m_block_impact_received;  //!< Block impact has been received.
  std::atomic_bool m_wall_impact_received;
  std::atomic_bool m_level_finished_received;  //!< Level has been successfully finished.
  std::atomic_bool m_explosion_received;  //!< Request for explosion received.
  std::atomic_bool m_prize_caught_received;  //!< Prize has been caught received.
  std::atomic_bool m_laser_beam_visibility_received;
  std::atomic_bool m_laser_block_impact_received;
  std::atomic_bool m_laser_pulse_received;
  std::atomic_bool m_ball_effect_received;
  /** @} */  // end of Mutex group

  /** @addtogroup Resources
   * @{
   */
  game::Resources* m_resources;
  /** @} */  // end of Resources group

// ----------------------------------------------
/* Private member-functions */
private:
  /** @defgroup ActiveObject Basic thread lifecycle and operation functions.
   * @{
   */
  void onStart() override final;  //!< Right after thread has been launched.
  void onStop() override final;   //!< Right before thread has been stopped.
  /// @brief Automatic check whether this thread should continue to operate.
  /// @return Whether this thread should continue sleeping (false)
  /// or working (true).
  bool checkForWakeUp() override final;
  /// @brief Operate the data or do some job as a response of incoming
  /// outer event.
  void eventHandler() override final;
  /** @} */  // end of ActiveObject group

  /** @defgroup Processors Actions being performed by SoundProcessor when
   *  corresponding event occurred and has been caught.
   *  @{
   */
  /// @brief Loads external resources into Graphic memory.
  void process_loadResources();
  /// @brief Plays sound when ball has been lost.
  void process_lostBall();
  /// @brief Plays sound when bite gets impacted.
  void process_biteImpact();
  /// @brief Plays sound when block gets impacted.
  void process_blockImpact();
  /// @brief Plays sound when wall gets impacted.
  void process_wallImpact();
  /// @brief Plays sound when level has been finished.
  void process_levelFinished();
  /// @brief Plays sound for particle system explosion.
  void process_explosion();
  /// @brief Plays sound for prize catching.
  void process_prizeCaught();
  /// @brief Plays sound when laser beam visibility changes.
  void process_laserBeamVisibility();
  /// @brief Plays sound when laser impacts a block.
  void process_laserBlockImpact();
  /// @brief Plays sound when laser pulse emerges.
  void process_laserPulse();
  /// @brief Plays sound for ball effect.
  void process_ballEffect();
  /** @} */  // end of Processors group

  /** @defgroup CoreFunc Core-related internal functionality.
   * @{
   */
  bool init();  //!< Initializes sound processor stuff.
  bool initPlayerQueue(int player_id);  //!< Initializes queue for sound buffers.
  bool playSound(const SoundBuffer* sound);  //!< Plays new sound, stopping any previous one.
  void destroy();  //!< Releases sound processor stuff.
  /** @} */  // end of CoreFunc group
};

/**
 * Error codes (SoundProcessor)
 *
 * 2001 - slCreateEngine() failed
 * 2002 - Realize() for Engine failed
 * 2003 - GetInterface() SL_IID_ENGINE failed
 * 2004 - CreateOutputMix() failed
 * 2005 - Realize() for Mixer failed
 */

}
}

#endif  // __ARKANOID_SOUND_PROCESSOR__H__
