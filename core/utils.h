// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden> except where
// otherwise noted.
// SPDX-License-Identifier: MIT
#ifndef __DONK_UTILS_H__
#define __DONK_UTILS_H__

namespace donk {
namespace util {

// Shamelessly stolen from
// https://en.wikipedia.org/wiki/Generator_(computer_programming)#C++
// Licensed under CC BY-SA 3.0
class int_range {
 public:
  int_range(int start, int end) : last(end), iter(start) {}

  const int_range& begin() const { return *this; }
  const int_range& end() const { return *this; }

  bool operator!=(const int_range&) const { return iter < last; }
  void operator++() { ++iter; }
  int operator*() const { return iter; }

 private:
  int last;
  int iter;
};

}  // namespace util
}  // namespace donk

#endif  // __DONK_UTILS_H__
