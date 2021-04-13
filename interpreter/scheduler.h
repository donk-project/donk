// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_PROJECT_DONK_INTERPRETER_SCHEDULER_H__
#define __DONK_PROJECT_DONK_INTERPRETER_SCHEDULER_H__

#include <deque>
#include <functional>

#include "cppcoro/generator.hpp"
#include "cppcoro/sync_wait.hpp"
#include "cppcoro/task.hpp"
#include "donk/core/iota.h"
#include "donk/core/procs.h"

// BYOND uses a task scheduler to permit blocking and non-blocking operations to
// share a single-threaded event loop.
//
// "Stephen001" on the BYOND forums describes it as below (2012-08-13):
//
// > The work-flow (in a basic sense) is this:
// >
// > - Whenever, really, you'll get things put on the queue from clients. Verbs
// >   called, spawns encountered in currently running code etc.
// > - Every tick, the scheduler looks at a list of stuff that needs doing, from
// >   the queue. It will look at things like scheduled time to see how "old"
// >   the piece of work is, and prioritise accordingly, but never do work for
// >   the next tick. So it only does work from "now" (this tick) or "the past"
// >   (previous ticks, where stuff is outstanding).
// > - The piece of code (proc, verb, spawned statement) is allowed to run,
// >   until: The statement completes (returns, errors etc), or the statement
// >   hits a sleep() or similar procedure that sleeps, like world to world
// >   Topic() stuff.
// > - If the statement hits a sleep, the work goes back in the queue, with a
// >   little bit of info added that says "I got to line X".
// > - If the statement entirely completed, it's removed from the queue.
// > - Rinse and repeat.

namespace donk {
class Interpreter;

namespace scheduler {
class RunningTask {
 public:
  RunningTask(transpiled_proc s, proc_args_t& a, running_proc_id id)
      : name("DONKAPI_SPAWN_PROC"), args(a), spawn(s), proc_id(id) {}
  RunningTask(std::shared_ptr<iota_t> i, std::string n, proc_args_t a,
              running_proc_id id)
      : iota(i), name(n), args(a), proc_id(id) {}

  std::shared_ptr<iota_t> iota;
  std::string name;
  proc_args_t args;
  int sleep_ticks_requested;
  int current_tick;
  running_proc task;
  transpiled_proc spawn;
  running_proc_id proc_id;
  std::shared_ptr<proc_ctxt_t> context;
  running_proc_info first_status_;
  // The last value yielded from this proc's coroutine.
  running_proc_info last_status_;
  std::shared_ptr<var_t> subtask_result_;
};

class TaskStack {
 public:
  void Push(std::shared_ptr<RunningTask> rt) { stack_.push_front(rt); }

  proc_callinstruct LastInstruction() {
    return stack_.front()->last_status_.callinstruct;
  }

  bool IsSleepElapsed(unsigned long last_tick) {
    return last_tick - stack_.front()->current_tick >=
           stack_.front()->sleep_ticks_requested;
  }

  int size() const { return stack_.size(); }

  bool empty() const { return size() == 0; }

  std::shared_ptr<RunningTask> front() { return stack_.front(); }

  std::shared_ptr<RunningTask> next() { return stack_.at(1); }

  void pop_front() { stack_.pop_front(); }

  std::deque<std::shared_ptr<RunningTask>>::iterator begin() {
    return stack_.begin();
  }

  std::deque<std::shared_ptr<RunningTask>>::iterator end() {
    return stack_.end();
  }

  std::string DEBUG__TopName() const { return stack_.front()->name; }

 private:
  std::deque<std::shared_ptr<RunningTask>> stack_;
};

class Scheduler {
 public:
  Scheduler(std::shared_ptr<donk::Interpreter> interpreter)
      : interpreter_(interpreter) {}

  bool empty() const { return queued_tasks_.empty() && running_tasks_.empty(); }

  running_proc_info& QueueProc(std::shared_ptr<iota_t> iota, std::string name,
                               proc_args_t args);
  running_proc_info& QueueChild(std::shared_ptr<iota_t> iota, std::string name,
                                proc_args_t args);
  running_proc_info& QueueSpawn(transpiled_proc spawn, proc_args_t& args);
  cppcoro::task<> Work(unsigned long last_tick);

  std::string DEBUG__OutputState() const;

 private:
  void ProcessRunningTask(std::shared_ptr<TaskStack> rt,
                          unsigned long last_tick);
  void PrepQueuedTask(std::shared_ptr<RunningTask> rt);

  std::shared_ptr<donk::Interpreter> interpreter_;
  std::deque<std::shared_ptr<TaskStack>> running_tasks_;
  std::deque<std::shared_ptr<RunningTask>> queued_tasks_;
  running_proc_id last_id_ = 1;
  std::shared_ptr<RunningTask> running_task_;
  running_proc_id running_id_;
  std::shared_ptr<TaskStack> running_stack_;
  unsigned long last_tick_;
};

}  // namespace scheduler
}  // namespace donk

#endif  // __DONK_PROJECT_DONK_INTERPRETER_SCHEDULER_H__
