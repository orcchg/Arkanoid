#ifndef __ARKANOID_PRIZE_PROCESSOR__H__
#define __ARKANOID_PRIZE_PROCESSOR__H__

#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include <GLES/gl.h>
#include <jni.h>

#include "ActiveObject.h"
#include "Bite.h"
#include "Event.h"
#include "EventListener.h"
#include "PrizePackage.h"

namespace game {

/// @class PrizeProcessor PrizeProcessor.h "include/PrizeProcessor.h"
/// @brief Standalone thread performs processing catch prizes.
class PrizeProcessor : public ActiveObject {
public:
  typedef PrizeProcessor* Ptr;

  PrizeProcessor(JavaVM* jvm);
  virtual ~PrizeProcessor() noexcept;

  /** @defgroup Callbacks These methods are responses of incoming events
   *  which PrizeProcessor subscribed on.
   *  @{
   */
  /// @brief Called when aspect ratio has been measured.
  void callback_aspectMeasured(float aspect);
  /// @brief Called when bite's dimensions have been measured.
  void callback_initBite(Bite bite);
  /// @brief Called when bite's location has changed.
  void callback_biteMoved(Bite moved_bite);
  /// @brief Called when prize has been generated.
  void callback_prizeReceived(PrizePackage package);
  /// @brief Called when prize has been located.
  void callback_prizeLocated(PrizePackage package);
  /// @brief Called when prize has gone.
  void callback_prizeHasGone(int prize_id);
  /** @} */  // end of Callbacks group

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
  inline void setOnPrizeCatchMethodID(jmethodID id) { fireJavaEvent_prizeCatch_id = id; };
  /** @} */  // end of JNIEnvironment group

// ----------------------------------------------
/* Public data-members */
public:
  /** @defgroup Event Outcoming events and listeners for incoming events.
   * @{
   */
  /// @brief Listens for measured aspect ratio.
  EventListener<float> aspect_ratio_listener;
  /// @brief Listens for bite's measured dimensions.
  EventListener<Bite> init_bite_listener;
  /// @brief Listens for bite location changes.
  EventListener<Bite> bite_location_listener;
  /// @brief Listens for event which occurs when prize has been generated.
  EventListener<PrizePackage> prize_listener;
  /// @brief Listens for prize location changes.
  EventListener<PrizePackage> prize_location_listener;
  /// @brief Listens whether prize has gone.
  EventListener<int> prize_gone_listener;

  /// @brief Notifies prize with specified ID has been caught.
  Event<PrizePackage> prize_caught_event;
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
  jmethodID fireJavaEvent_prizeCatch_id;
  /** @} */  // end of JNIEnvironment group

  /** @defgroup LogicData Game logic related data members.
   * @{
   */
  GLfloat m_aspect;  //!< Measured aspect ratio.
  Bite m_bite;  //!< Physical bite's representation.
  GLfloat m_bite_upper_border;  //!< Upper border of bite.
  std::unordered_map<int, PrizePackage> m_prize_packages;
  std::unordered_set<int> m_removed_prizes;
  /** @} */  // end of LogicData group

  /** @defgroup Mutex Thread-safety variables
   * @{
   */
  std::mutex m_jnienvironment_mutex;  //!< Sentinel for thread attach to JVM.
  std::mutex m_aspect_ratio_mutex;  //!< Sentinel for measured aspect ratio.
  std::mutex m_init_bite_mutex;  //!< Sentinel for initial bite dimensions.
  std::mutex m_bite_location_mutex;  //!< Sentinel for bite's center location changes.
  std::mutex m_prize_mutex;  //!< Sentinel for prize receiving.
  std::mutex m_prize_location_mutex;  //!< Sentinel for prize location changes.
  std::mutex m_prize_gone_mutex;  //!< Sentinel for prize has gone.
  std::atomic_bool m_aspect_ratio_received;  //!< Aspect ratio has been measured.
  std::atomic_bool m_init_bite_received;  //!< Initial bite dimensions have been received.
  std::atomic_bool m_bite_location_received;  //!< New bite's center location has been received.
  std::atomic_bool m_prize_received;  //!< Prize has been received.
  std::atomic_bool m_prize_location_received;  //!< Prize location has been received.
  std::atomic_bool m_prize_gone_received;  //!< Prize has gone received.
  /** @} */  // end of Mutex group

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

  /** @defgroup Processors Actions being performed by PrizeProcessor when
   *  corresponding event occurred and has been caught.
   *  @{
   */
  /// @brief Processing when aspect ratio has been measured.
  void process_aspectMeasured();
  /// @brief Set's the bite's measured dimensions.
  void process_initBite();
  /// @brief Processing when bite's location has changed.
  void process_biteMoved();
  /// @brief Processing prize generation.
  void process_prizeReceived();
  /// @brief Processing prize relocation.
  void process_prizeLocated();
  /// @brief Processing prize has gone.
  void process_prizeHasGone();
  /** @} */  // end of Processors group

  /** @defgroup LogicFunc Game logic related member functions.
   * @{
   */
  /// @brief Adds prize to be removed later.
  void addPrizeToRemoved(int prize_id);
  /// @brief Clears removed prizes.
  void clearRemovedPrizes();
  /// @brief Notifies Java layer prize has been caught.
  void onPrizeCatch(int prize_id);
  /** @} */  // end of LogicFunc group
};

}

#endif  // __ARKANOID_PRIZE_PROCESSOR__H__
