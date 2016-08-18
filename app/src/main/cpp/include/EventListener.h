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

#ifndef SURFACE3D_EVENTLISTENER__H__
#define SURFACE3D_EVENTLISTENER__H__

#include <iostream>
#include <memory>
#include "logger.h"


template <typename E>
class ListenerBinder;

template <typename E>
class EventListener {
  friend class ListenerBinder<E>;

public:
  EventListener() {}

  virtual ~EventListener() {
    unbind();
  };

  EventListener(const EventListener&) = delete;
  EventListener& operator=(const EventListener& rhs) = delete;

  EventListener& operator=(std::shared_ptr<ListenerBinder<E>> el_ptr) {
    if (isBinded()) {  // unsubscribe from event
      this->unbind();
    }
    this->binder = el_ptr;
    el_ptr->eventListener = this;
    return *this;
  };

  bool isBinded() const {
    std::shared_ptr<ListenerBinder<E>> my_binder(binder.lock());
    if (my_binder) {
      return true;
    } else {
      return false;
    }
  }

private:
  std::weak_ptr<ListenerBinder<E>> binder;

  void unbind() {
    std::shared_ptr<ListenerBinder<E>> my_binder(binder.lock());
    if (my_binder) {
      bool result = my_binder->unbindFromEvent();
      if (!result) {
        ERR("Cannot unbind from event!");
      }
    }
  }
};


#endif  // SURFACE3D_EVENTLISTENER__H__
