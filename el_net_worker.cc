// Copyright (c) 2013 ASMlover. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list ofconditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materialsprovided with the
//    distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#include "el_net_internal.h"
#include "el_time.h"
#include "el_event_poller.h"
#include "el_net_worker.h"

#define POLL_TIMEOUT  (10)

namespace el {

NetWorker::NetWorker(void)
  : running_(false)
  , poller_(nullptr)
  , thread_(nullptr)
  , dispatcher_(nullptr) {
}

NetWorker::~NetWorker(void) {
  Stop();
}

bool NetWorker::Start(void) {
  if (nullptr == dispatcher_)
    return false;

  if (nullptr == (poller_ = new EventPoller()))
    return false;

  do {
    thread_ = new Thread(&NetWorker::Routine, this);
    if (nullptr == thread_)
      break;

    running_ = true;
    thread_->Start();

    return true;
  } while (0);

  Stop();
  return false;
}

void NetWorker::Stop(void) {
  running_ = false;

  if (nullptr != thread_) {
    thread_->Join();

    delete thread_;
    thread_ = nullptr;
  }

  if (nullptr != poller_) {
    delete poller_;
    poller_ = nullptr;
  }
}

bool NetWorker::AddConnector(Connector* conn) {
  if (nullptr == poller_ || nullptr == conn)
    return false;

  if (!poller_->Insert(conn))
    return false;

  poller_->AddEvent(conn, kEventTypeRead);

  return true;
}

void NetWorker::Routine(void* argument) {
  NetWorker* self = static_cast<NetWorker*>(argument);
  if (nullptr == self || nullptr == self->poller_ || nullptr == self->dispatcher_)
    return;

  while (self->running_) {
    if (!self->poller_->Dispatch(self->dispatcher_, POLL_TIMEOUT)) {
      el::Sleep(1);
      continue;
    }
  }
}

}
