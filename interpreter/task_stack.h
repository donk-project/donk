// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_PROJECT_DONK_INTERPRETER_TASK_STACK_H__
#define __DONK_PROJECT_DONK_INTERPRETER_TASK_STACK_H__

#include <deque>
#include <functional>

#include "cppcoro/generator.hpp"
#include "cppcoro/sync_wait.hpp"
#include "cppcoro/task.hpp"
#include "donk/core/iota.h"
#include "donk/core/procs.h"

namespace donk {
namespace scheduler {

// TaskStack is where a running task is stored, with child tasks pushed onto
// the stack when running.
class TaskStack {
 public:
  void Push(std::shared_ptr<RunningTask> rt);
  bool IsSleepElapsed(unsigned long last_tick);
  proc_callinstruct LastInstruction();

  int size() const;
  bool empty() const;
  void pop_front();

  std::shared_ptr<RunningTask> front();
  std::shared_ptr<RunningTask> next();

  std::deque<std::shared_ptr<RunningTask>>::iterator begin();
  std::deque<std::shared_ptr<RunningTask>>::iterator end();

  std::string DEBUG__TopName() const;

 private:
  std::deque<std::shared_ptr<RunningTask>> stack_;
};

}  // namespace scheduler
}  // namespace donk

#endif  // __DONK_PROJECT_DONK_INTERPRETER_TASK_STACK_H__
