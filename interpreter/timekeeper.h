// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_INTERPRETER_TIMEKEEPER_H__
#define __DONK_INTERPRETER_TIMEKEEPER_H__

#include <chrono>
#include <ratio>

#include "spdlog/spdlog.h"

namespace donk {
namespace scheduler {

class TimeKeeper {
 public:
  TimeKeeper() {}

  void Start() { start_ = std::chrono::steady_clock::now(); }

  bool Advanced() { return MsSinceLastTick() >= 100; }

  unsigned long Tick() {
    last_tick_ = std::chrono::steady_clock::now();
    tick_count_++;
    return tick_count_;
  }

  // TODO: These operations are probably expensive and should be memoized to the
  // extent possible
  unsigned long MsSinceStart() {
    auto x = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start_);
    return x.count();
  }

  unsigned long MsSinceLastTick() {
    auto x = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - last_tick_);
    return x.count();
  }

  unsigned long TicksSinceStart() { return tick_count_; }

 private:
  std::chrono::time_point<std::chrono::steady_clock> start_;
  std::chrono::time_point<std::chrono::steady_clock> last_tick_;
  unsigned long tick_count_;
};

}  // namespace scheduler
}  // namespace donk

#endif  // __DONK_INTERPRETER_TIMEKEEPER_H__
