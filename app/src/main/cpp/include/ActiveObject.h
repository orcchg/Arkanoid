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

#ifndef SURFACE3D_ACTIVE_OBJECT__H__
#define SURFACE3D_ACTIVE_OBJECT__H__

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

#include "Event.h"


class ActiveObject {
public:
	ActiveObject()
		: m_is_detached(false)
		, m_is_runnning(false)
		, m_continue_running(false) {
		m_main_thread = nullptr;
	}

	virtual ~ActiveObject() {
		__join__();
		delete m_main_thread;
	}

	void launch() {
		if (!m_is_runnning) {
		  __launch__();
		}
	}

	void stop() {
		if (m_main_thread != nullptr) {
	    this->__stop__();
		}
	}

	//@brief call this to let ActiveObject perform it's job
	void interrupt() {
		std::lock_guard<std::mutex> lock(m_wake_up_mutex);
		m_wake_up_condition.notify_one();
	}

	void sleep(int milliseconds) {
	  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	Event<bool> object_has_launched_event;
	Event<bool> object_has_stopped_event;

protected:
	std::thread* m_main_thread;
	std::mutex m_wake_up_mutex;
	std::condition_variable m_wake_up_condition;
	std::atomic_bool m_continue_running;
	bool m_is_runnning;
	bool m_is_detached;

	void __launch__() {
		m_continue_running.store(true);
		m_main_thread = new std::thread(
		  [this]() {
			this->onStart();
			this->__run__();
			this->onStop();
		});
		m_is_runnning = true;
	}

	void __detach__() {
    if (m_main_thread != nullptr && m_is_runnning) {
      if (!m_is_detached) {
        m_main_thread->detach();
        m_is_detached = true;
      }
    }
	}

	void __join__() {
	  if (__joinable__()) {
	    m_main_thread->join();
	    m_is_runnning = false;
	  }
	}

	void __stop__() {
		{
		  m_continue_running.store(false);
		  std::lock_guard<std::mutex> lock(m_wake_up_mutex);
		  m_wake_up_condition.notify_one();
		}
		__join__();
	}

  bool __joinable__() const {
    if (m_main_thread != nullptr) {
      return m_main_thread->joinable();
    } else {
      return false;
    }
  }

  /**
   * ActiveObject instance (*this) just waits for job to do (specified in eventHandler())
   * or stop requested, these two case should be considered separately:
   *
   * 1.  When some data necessary for job has been prepared and / or some
   *     prerequisites has been fulfilled, the 'preparator' thread must invoke
   *     interrupt() on ActiveObject to let the processing be done. This leads ActiveObject
   *     to perform processing only if a sort of wake-up condition check is passed.
   *     ActiveObject does not perform an idle run, according to predicate's rhs-value.
   *
   * 2.  When calling thread has requested ActiveObject to stop (via stop()),
   *     ActiveObject may be somewhere in the middle of __run__() method:
   *
   *     a) If it is right before first while(), it skips the loop and leaves from __run__().
   *
   *     b) If it is inside first while(), before wait() invocation, it doesn't blocks due
   *        to 'true' in predicate. Then it also ignores second while() and leaves from __run__().
   *
   *     c) If it hasn't blocked on wait(), it ignores second while() and leaves from __run__().
   *
   *     d) If it has blocked on wait(), it wakes up and then ignores second while() and leaves.
   */
  virtual void __run__() {
    while (m_continue_running) {
      {
        std::unique_lock<std::mutex> lock(m_wake_up_mutex);
        m_wake_up_condition.wait(lock, [this](){ return this->checkForWakeUp() | !this->m_continue_running; });
      }
      while (checkForWakeUp() & this->m_continue_running) {
        eventHandler();
      }
    }
  }

	//@brief called from new thread after launch is called
	virtual void onStart() {
		object_has_launched_event.notifyListeners(true);
	}

	//@brief called from module's thread before join
	virtual void onStop() {
		object_has_stopped_event.notifyListeners(true);
	}

	virtual bool checkForWakeUp() = 0;
	virtual void eventHandler() = 0;
};

#endif  //  SURFACE3D_ACTIVE_OBJECT__H__
