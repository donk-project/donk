// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/interpreter/task_stack.h"

#include "donk/interpreter/running_task.h"
#include "spdlog/spdlog.h"

namespace donk {
namespace scheduler {

void TaskStack::Push(std::shared_ptr<RunningTask> rt) { stack_.push_front(rt); }

proc_callinstruct TaskStack::LastInstruction() {
  return stack_.front()->last_status().callinstruct;
}

bool TaskStack::IsSleepElapsed(unsigned long last_tick) {
  return last_tick - stack_.front()->current_tick() >=
         stack_.front()->sleep_ticks_requested();
}

int TaskStack::size() const { return stack_.size(); }

bool TaskStack::empty() const { return size() == 0; }

std::shared_ptr<RunningTask> TaskStack::front() { return stack_.front(); }

std::shared_ptr<RunningTask> TaskStack::next() { return stack_.at(1); }

void TaskStack::pop_front() { stack_.pop_front(); }

std::deque<std::shared_ptr<RunningTask>>::iterator TaskStack::begin() {
  return stack_.begin();
}

std::deque<std::shared_ptr<RunningTask>>::iterator TaskStack::end() {
  return stack_.end();
}

std::string TaskStack::DEBUG__TopName() const { return stack_.front()->name(); }

}  // namespace scheduler
}  // namespace donk
