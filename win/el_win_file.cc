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
#include "../elib_internal.h"
#include "../el_file.h"

namespace el {

File::File(void)
  : fd_(INVALID_HANDLE_VALUE)
  , buffer_(nullptr)
  , allocated_(false)
  , buf_size_(0)
  , data_size_(0) {
}

File::~File(void) {
  Close();
}

void File::SetBuffer(char* buffer, size_t size) {
  // this function must be used before any Write
  if (nullptr != buffer_ && allocated_)
    free(buffer_);

  data_size_ = 0;
  buf_size_ = size;
  if (nullptr == buffer) {
    buffer_ = (char*)malloc(buf_size_);
    allocated_ = true;
  }
  else {
    buffer_ = buffer;
    allocated_ = false;
  }
}

bool File::Open(const char* fname, bool append) {
  fd_ = CreateFileA(fname,
    GENERIC_READ | GENERIC_WRITE,
    FILE_SHARE_READ | FILE_SHARE_WRITE,
    nullptr,
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    nullptr);
  if (INVALID_HANDLE_VALUE == fd_)
    return false;

  if (append)
    SetFilePointer(fd_, 0, nullptr, FILE_END);
  buf_size_ = BUFFERSIZE;
  buffer_ = (char*)malloc(buf_size_);
  if (nullptr == buffer_)
    goto Exit;
  allocated_ = true;
  data_size_ = 0;

  return true;

Exit:
  CloseHandle(fd_);
  fd_ = INVALID_HANDLE_VALUE;
  return false;
}

void File::Close(void) {
  if (nullptr != buffer_) {
    if (data_size_ > 0 && INVALID_HANDLE_VALUE != fd_) {
      DWORD ret;
      WriteFile(fd_, buffer_, data_size_, &ret, nullptr);
    }

    if (allocated_) {
      free(buffer_);
      allocated_ = false;
    }
    buffer_ = nullptr;
  }
  data_size_ = 0;

  if (INVALID_HANDLE_VALUE != fd_) {
    CloseHandle(fd_);
    fd_ = INVALID_HANDLE_VALUE;
  }
}

size_t File::Write(const void* buffer, size_t size) {
  if (nullptr == buffer || 0 == size)
    return 0;

  size_t free_size = buf_size_ - data_size_;
  if (free_size > size) {
    memcpy(buffer_ + data_size_, buffer, size);
    data_size_ += size;
  }
  else {
    DWORD ret;
    size_t copy_size = size - free_size;
    memcpy(buffer_ + data_size_, buffer, free_size);
    WriteFile(fd_, buffer_, buf_size_, &ret, nullptr);
    data_size_ = 0;

    if (copy_size > 0) {
      memcpy(buffer_ + data_size_, (char*)buffer + free_size, copy_size);
      data_size_ += copy_size;
    }
  }

  return size;
}

}
