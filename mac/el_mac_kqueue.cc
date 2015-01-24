// Copyright (c) 2015 ASMlover. All rights reserved.
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
#include "../el_net_internal.h"
#include "../el_socket.h"
#include "../el_connector.h"
#include "el_mac_kqueue.h"

namespace el {

Kqueue::Kqueue(void)
  : kqueue_fd_(kNetTypeInval)
  , event_count_(kEventCount)
  , events_(nullptr) {
  if (!Init())
    abort();
}

Kqueue::~Kqueue(void) {
  Destroy();
}

bool Kqueue::Init(void) {
  kqueue_fd_ = kqueue();
  if (kNetTypeInval == kqueue_fd_)
    return false;

  event_count_ = kEventCount;
  size_t size = sizeof(struct kevent) * event_count_;

  do {
    events_ = (struct kevent*)malloc(size);
    if (nullptr == events_)
      break;

    return true;
  } while (0);

  Destroy();
  return false;
}

void Kqueue::Destroy(void) {
  if (nullptr != events_) {
    free(events_);
    events_ = nullptr;
  }
  event_count_ = kEventCount;

  if (kNetTypeInval != kqueue_fd_) {
    close(kqueue_fd_);
    kqueue_fd_ = kNetTypeInval;
  }
}

bool Kqueue::Regrow(void) {
  uint32_t new_event_count = (0 != event_count_ ?
      2 * event_count_ : kEventCount);
  size_t size = sizeof(struct kevent) * new_event_count;

  events_ = (struct kevent*)realloc(events_, size);
  if (nullptr == events_) {
    abort();
    return false;
  }

  event_count_ = new_event_count;

  return true;
}

bool Kqueue::Insert(Connector* conn) {
  if (nullptr == conn)
    return false;

  return true;
}

void Kqueue::Remove(Connector* conn) {
  if (nullptr == conn)
    return;

  struct kevent event;
  EV_SET(&event, conn->fd(), EVFILT_READ, EV_DELETE, 0, 0, nullptr);
  kevent(kqueue_fd_, &event, 1, nullptr, 0, nullptr);
  EV_SET(&event, conn->fd(), EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
  kevent(kqueue_fd_, &event, 1, nullptr, 0, nullptr);
}

bool Kqueue::AddEvent(Connector* conn, int ev) {
  if (nullptr == conn)
    return false;

  struct kevent event;
  if (ev & kEventTypeRead)
    EV_SET(&event, conn->fd(), EVFILT_READ, EV_ADD, 0, 0, conn);

  if (ev & kEventTypeWrite)
    EV_SET(&event, conn->fd(), EVFILT_WRITE, EV_ADD, 0, 0, conn);

  if (kNetTypeError ==
      kevent(kqueue_fd_, &event, 1, nullptr, 0, nullptr))
    return false;

  return true;
}

bool Kqueue::DelEvent(Connector* conn, int ev) {
  if (nullptr == conn)
    return false;

  struct kevent event;
  if (ev & kEventTypeRead)
    EV_SET(&event, conn->fd(), EVFILT_READ, EV_DELETE, 0, 0, nullptr);

  if (ev & kEventTypeWrite)
    EV_SET(&event, conn->fd(), EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);

  if (kNetTypeError ==
      kevent(kqueue_fd_, &event, 1, nullptr, 0, nullptr))
    return false;

  return true;
}

bool Kqueue::Dispatch(Dispatcher* dispatcher, uint32_t millitm) {
  if (nullptr == dispatcher)
    return false;

  struct timespec ts = {millitm / 1000, (millitm % 1000) * 1000000};
  int num = kevent(kqueue_fd_, nullptr, 0, events_, event_count_, &ts);
  if (kNetTypeError == num || 0 == num)
    return false;

  Connector* conn;
  for (int i = 0; i < num; ++i) {
    conn = (Connector*)events_[i].udata;
    if (nullptr == conn)
      continue;

    if (kNetTypeInval == conn->fd())
      continue;
    if (events_[i].flags & EV_EOF)
      dispatcher->DispatchReader(this, conn);

    if (kNetTypeInval == conn->fd())
      continue;
    if (events_[i].flags & EVFILT_READ)
      dispatcher->DispatchReader(this, conn);

    if (kNetTypeInval == conn->fd())
      continue;
    if (events_[i].flags & EVFILT_WRITE)
      dispatcher->DispatchWriter(this, conn);
  }

  if (static_cast<uint32_t>(num) == event_count_)
    Regrow();

  return true;
}

}
