// MIT License

// Copyright (c) 2017 Zhuhao Wang

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef NEKIT_UTILS_BUFFER
#define NEKIT_UTILS_BUFFER

#include <cstddef>
#include <cstdint>

#include <boost/noncopyable.hpp>

#include "buffer_reserve_size.h"

namespace nekit {
namespace utils {
struct Buffer final : public boost::noncopyable {
 public:
  Buffer(std::size_t size)
      : size_(size), data_(::operator new(size)), front_(0), back_(0) {}

  Buffer(BufferReserveSize size) : Buffer(size, 0) {}

  Buffer(BufferReserveSize size, std::size_t content)
      : Buffer(size.prefix() + size.suffix() + content) {
    ReserveFront(size.prefix());
    ReserveBack(size.suffix());
  }

  ~Buffer() { ::operator delete(data_); }

  // Return the underlying buffer.
  void *data() { return data_; }
  const void *data() const { return data_; }
  std::size_t size() const { return size_; }

  bool ReserveFront(std::size_t size) {
    // Be careful. Overflow is not checked.
    if (size >= size_ || size + front_ + back_ > size_) {
      return false;
    }

    front_ += size;
    return true;
  }

  bool ReleaseFront(std::size_t size) {
    if (front_ < size) {
      return false;
    }

    front_ -= size;
    return true;
  }

  bool ReserveBack(std::size_t size) {
    // Be careful. Overflow is not checked.
    if (size >= size_ || size + front_ + back_ > size_) {
      return false;
    }

    back_ += size;
    return true;
  }

  bool ReleaseBack(std::size_t size) {
    if (back_ < size) {
      return false;
    }

    back_ -= size;
    return true;
  }

  std::size_t capacity() const { return size_ - front_ - back_; }

  void *buffer() { return static_cast<uint8_t *>(data_) + front_; }

  const void *buffer() const { return static_cast<uint8_t *>(data_) + front_; }

 private:
  const std::size_t size_;
  void *const data_;

  std::size_t front_;
  std::size_t back_;
};
}  // namespace utils
}  // namespace nekit

#endif /* NEKIT_UTILS_BUFFER */
