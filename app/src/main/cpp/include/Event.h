/**
 * Created by Morten Nobel-Jørgensen on 8/18/13
 * Modified by Alov Maxim <alovmax@yandex.ru>
 *
 * Copyright (c) 2013 Morten, 2015 Alov Maxim
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

#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <list>

#include "EventListener.h"
#include "ListenerBinder.h"
#include "logger.h"


template <typename E>
class Event {
public:
	Event() : eventListenerId(0) {}
	virtual ~Event() {}

	typename ListenerBinder<E>::Ptr createListener(std::function<void (E)> listener){
	  typename ListenerBinder<E>::Ptr retval = std::make_shared<ListenerBinder<E>>(this, listener, eventListenerId);
	  listeners.emplace_back(retval);
	  eventListenerId++;
	  return retval;
	}

	template <typename P, typename Func>
	typename ListenerBinder<E>::Ptr createListener(Func f, P p) {
	  std::function<void(E)> func = std::bind(f, p, std::placeholders::_1);
	  return createListener(func);
	}

	void notifyListeners(E e){
	  for(auto & l : listeners) {
	    l.binder_ptr->callListenerSafe(e);
	  }
	}

	bool removeListener(int id){
	  auto iter = listeners.begin();
	  while (iter != listeners.end()) {
	    if (iter->getId() == id){
	      listeners.erase(iter);
	      return true;
	    }
	    iter++;
	  }
	  return false;
	}

	void clearListeners() {
	  listeners.clear();
	}

	bool hasListeners() const {
	  return !listeners.empty();
	}

	int getListenersCount() const {
	  return listeners.size();
	}

protected:
  struct EventGlue {
    typename ListenerBinder<E>::Ptr binder_ptr;

    EventGlue(typename ListenerBinder<E>::Ptr binder) : binder_ptr(binder) {}

    ~EventGlue() {
      binder_ptr->event = nullptr;
    }

    inline const int& getId() const { return binder_ptr->getId(); }
  };

  std::list<EventGlue> listeners;
	int eventListenerId;
};
