#include "Exceptions.h"
#include "logger.h"
#include "Params.h"
#include "PrizeProcessor.h"

namespace game {

PrizeProcessor::PrizeProcessor(JavaVM* jvm)
  : m_jvm(jvm), m_jenv(nullptr)
  , master_object(nullptr)
  , m_aspect(1.0f)
  , m_bite()
  , m_bite_upper_border(-BiteParams::neg_biteElevation)
  , m_prize_packages()
  , m_removed_prizes() {

  DBG("enter PrizeProcessor ctor");
  m_aspect_ratio_received.store(false);
  m_init_bite_received.store(false);
  m_bite_location_received.store(false);
  m_prize_received.store(false);
  m_prize_location_received.store(false);
  m_prize_gone_received.store(false);

  m_removed_prizes.reserve(24);
  DBG("exit PrizeProcessor ctor");
}

PrizeProcessor::~PrizeProcessor() {
  DBG("enter PrizeProcessor ~dtor");
  m_jvm = nullptr;  m_jenv = nullptr;
  DBG("exit PrizeProcessor ~dtor");
}

/* Callbacks group */
// ----------------------------------------------------------------------------
void PrizeProcessor::callback_aspectMeasured(float aspect) {
  std::unique_lock<std::mutex> lock(m_aspect_ratio_mutex);
  m_aspect_ratio_received.store(true);
  m_aspect = aspect;
  interrupt();
}

void PrizeProcessor::callback_initBite(Bite bite) {
  std::unique_lock<std::mutex> lock(m_init_bite_mutex);
  m_init_bite_received.store(true);
  m_bite = bite;
  interrupt();
}

void PrizeProcessor::callback_biteMoved(Bite moved_bite) {
  std::unique_lock<std::mutex> lock(m_bite_location_mutex);
  m_bite_location_received.store(true);
  m_bite = moved_bite;
  interrupt();
}

void PrizeProcessor::callback_prizeReceived(PrizePackage package) {
  std::unique_lock<std::mutex> lock(m_prize_mutex);
  m_prize_received.store(true);
  m_prize_packages[package.getID()] = package;
  interrupt();
}

void PrizeProcessor::callback_prizeLocated(PrizePackage package) {
  std::unique_lock<std::mutex> lock(m_prize_location_mutex);
  m_prize_location_received.store(true);
  m_prize_packages[package.getID()] = package;
  interrupt();
}

void PrizeProcessor::callback_prizeHasGone(int prize_id) {
  std::unique_lock<std::mutex> lock(m_prize_gone_mutex);
  m_prize_gone_received.store(true);
  m_prize_packages[prize_id].setGone(true);
  addPrizeToRemoved(prize_id);
  interrupt();
}

/* *** Private methods *** */
/* JNIEnvironment group */
// ----------------------------------------------------------------------------
void PrizeProcessor::attachToJVM() {
  std::unique_lock<std::mutex> lock(m_jnienvironment_mutex);
  auto result = m_jvm->AttachCurrentThread(&m_jenv, nullptr /* thread args */);
  if (result != JNI_OK) {
    ERR("PrizeProcessor thread was not attached to JVM !");
    throw JNI_exception();
  }
}

void PrizeProcessor::detachFromJVM() {
  std::unique_lock<std::mutex> lock(m_jnienvironment_mutex);
  m_jvm->DetachCurrentThread();
}

/* ActiveObject group */
// ----------------------------------------------------------------------------
void PrizeProcessor::onStart() {
  DBG("PrizeProcessor onStart");
  attachToJVM();
}

void PrizeProcessor::onStop() {
  DBG("PrizeProcessor onStop");
  detachFromJVM();
}

bool PrizeProcessor::checkForWakeUp() {
  return m_aspect_ratio_received.load()||
         m_init_bite_received.load() ||
         m_bite_location_received.load() ||
         m_prize_received.load() ||
         m_prize_location_received.load() ||
         m_prize_gone_received.load();
}

void PrizeProcessor::eventHandler() {
  if (m_aspect_ratio_received.load()) {
    m_aspect_ratio_received.store(false);
    process_aspectMeasured();
  }
  if (m_init_bite_received.load()) {
    m_init_bite_received.store(false);
    process_initBite();
  }
  if (m_bite_location_received.load()) {
    m_bite_location_received.store(false);
    process_biteMoved();
  }
  if (m_prize_received.load()) {
    m_prize_received.store(false);
    process_prizeReceived();
  }
  if (m_prize_location_received.load()) {
    m_prize_location_received.store(false);
    process_prizeLocated();
  }
}

/* Processors group */
// ----------------------------------------------------------------------------
void PrizeProcessor::process_aspectMeasured() {
  std::unique_lock<std::mutex> lock(m_aspect_ratio_mutex);
  // no-op
}

void PrizeProcessor::process_initBite() {
  std::unique_lock<std::mutex> lock(m_init_bite_mutex);
  m_bite_upper_border = -BiteParams::neg_biteElevation;
}

void PrizeProcessor::process_biteMoved() {
  std::unique_lock<std::mutex> lock(m_bite_location_mutex);
  // no-op
}

void PrizeProcessor::process_prizeReceived() {
  std::unique_lock<std::mutex> lock(m_prize_mutex);
  // no-op
}

void PrizeProcessor::process_prizeLocated() {
  std::unique_lock<std::mutex> lock(m_prize_location_mutex);
  clearRemovedPrizes();
  for (auto& item : m_prize_packages) {
    if (!item.second.hasGone() &&
        item.second.getY() <= m_bite_upper_border + PrizeParams::prizeHalfHeight * m_aspect &&
        item.second.getY() >= m_bite_upper_border - (BiteParams::biteHeight + PrizeParams::prizeHalfHeight) * m_aspect &&
        item.second.getX() >= -(m_bite.getDimens().halfWidth() + PrizeParams::prizeHalfWidth) + m_bite.getXPose() &&
        item.second.getX() <= (m_bite.getDimens().halfWidth() + PrizeParams::prizeHalfWidth) + m_bite.getXPose()) {
      auto prize_id = item.second.getID();
      addPrizeToRemoved(prize_id);
      prize_caught_event.notifyListeners(item.second);
      onPrizeCatch(prize_id);
    }
  }
}

void PrizeProcessor::process_prizeHasGone() {
  std::unique_lock<std::mutex> lock(m_prize_gone_mutex);
  for (auto& item : m_removed_prizes) {
    m_prize_packages.erase(item);
  }
  m_removed_prizes.clear();
}

/* LogicFunc group */
// ----------------------------------------------------------------------------
void PrizeProcessor::addPrizeToRemoved(int prize_id) {
  m_removed_prizes.insert(prize_id);
}

void PrizeProcessor::clearRemovedPrizes() {
  for (auto& item : m_removed_prizes) {
    m_prize_packages.erase(item);
  }
  m_removed_prizes.clear();
}

void PrizeProcessor::onPrizeCatch(int prize_id) {
  int prize = static_cast<int>(Prize::NONE);
  auto it = m_prize_packages.find(prize_id);
  if (it != m_prize_packages.end()) {
    prize = static_cast<int>(m_prize_packages.at(prize_id).getPrize());
  }
  m_jenv->CallVoidMethod(master_object, fireJavaEvent_prizeCatch_id, prize);
}

}
