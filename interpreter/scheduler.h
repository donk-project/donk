// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_INTERPRETER_SCHEDULER_H__
#define __DONK_INTERPRETER_SCHEDULER_H__

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
// namespace internal {
class Interpreter;
// }
namespace scheduler {

class RunningTask {
 public:
  RunningTask(std::shared_ptr<iota_t> i, std::string n, proc_args_t a,
              running_proc_id id)
      : iota(i), name(n), args(a), proc_id(id) {}

  std::shared_ptr<iota_t> iota;
  std::string name;
  proc_args_t args;
  int sleep_ticks_requested;
  int current_tick;
  running_proc task;
  running_proc_id proc_id;
  std::shared_ptr<proc_ctxt_t> context;
  std::vector<running_proc_id> waiting_tasks;
  // The last value yielded from this proc's coroutine.
  running_proc_info last_status_;
  std::function<void()> callback_;
};

class Scheduler {
 public:
  Scheduler(std::shared_ptr<donk::Interpreter> interpreter)
      : interpreter_(interpreter) {}

  bool empty() const { return queued_tasks_.empty(); }

  running_proc_id QueueProc(std::shared_ptr<iota_t> iota, std::string name,
                            proc_args_t args) {
    last_id_++;
    std::shared_ptr<RunningTask> rt =
        std::make_shared<RunningTask>(iota, name, args, last_id_);
    queued_tasks_.push_back(rt);
    return last_id_;
  }

  cppcoro::task<> Work(unsigned long last_tick) {
    int len = running_tasks_.size();
    if (len > 0) {
      int i = 0;
      while (i < len) {
        auto rt = running_tasks_.front();
        running_tasks_.pop_front();
        if (rt->last_status_.callinstruct ==
            proc_callinstruct::kSleepRequested) {
          if (last_tick - rt->current_tick >= rt->sleep_ticks_requested) {
            ProcessRunningTask(rt, last_tick);
          } else {
            running_tasks_.emplace_back(rt);
          }
        } else if (rt->last_status_.callinstruct ==
                   proc_callinstruct::kAwaitingChild) {
          if (rt->waiting_tasks.empty()) {
            ProcessRunningTask(rt, last_tick);
          } else {
            running_tasks_.emplace_back(rt);
          }
        } else {
          ProcessRunningTask(rt, last_tick);
        }

        i++;
      }
    }

    if (queued_tasks_.empty()) {
      co_return;
    }

    while (!queued_tasks_.empty()) {
      // Check for un-running, queued tasks, and add them to the run queue.
      auto rt = queued_tasks_.front();
      queued_tasks_.pop_front();
      auto proc = rt->iota->proc_table().GetProcByName(rt->name);
      auto func = proc.GetInternalFunc();

      // TODO: Fix assignment of src and usr
      auto ctxt = std::make_shared<proc_ctxt_t>(interpreter_);
      ctxt->AssignSrc(rt->iota);
      ctxt->AssignUsr(rt->iota);

      auto awaiter = func(*ctxt, rt->args);
      rt->task = std::move(awaiter);
      rt->context = ctxt;
      rt->current_tick = last_tick;

      ProcessRunningTask(rt, last_tick);
    }
  }

 private:
  std::shared_ptr<RunningTask> FindTask(running_proc_id id) {
    for (int i = 0; i < running_tasks_.size(); i++) {
      auto rt = running_tasks_[i];
      if (rt->proc_id == id) {
        return rt;
      }
    }
    for (int i = 0; i < queued_tasks_.size(); i++) {
      auto rt = queued_tasks_[i];
      if (rt->proc_id == id) {
        return rt;
      }
    }
    return nullptr;
  }

  void ProcessRunningTask(std::shared_ptr<RunningTask> rt,
                          unsigned long last_tick) {
    for (auto result : rt->task) {
      if (result.callinstruct == proc_callinstruct::kSleepRequested) {
        rt->sleep_ticks_requested = result.requested_tick_delay;
        rt->last_status_ = result;
        rt->current_tick = last_tick;
        running_tasks_.emplace_back(rt);
        return;
      } else if (result.callinstruct == proc_callinstruct::kAwaitingChild) {
        // The calling proc rt has spun off a child proc, which we must now
        // wait for before resuming.
        rt->waiting_tasks.push_back(result.childproc);
        rt->last_status_ = result;
        running_tasks_.emplace_back(rt);

        // Add a callback to the child task to remove itself from the parent's
        // list of awaited tasks.
        auto child_task = FindTask(result.childproc);
        if (child_task != nullptr) {
          child_task->callback_ = [=]() {
            rt->context->ChildResult(child_task->context->result());
            rt->waiting_tasks.erase(std::remove_if(
                rt->waiting_tasks.begin(), rt->waiting_tasks.end(),
                [=](running_proc_id r2) { return r2 == result.childproc; }));
          };
        } else {
          spdlog::critical(
              "Proc {} requested delay for child {} but it could not be found",
              rt->name, result.childproc);
        }
        return;
      }
    }
    if (rt->callback_ != nullptr) {
      rt->callback_();
    }
  }

  std::shared_ptr<donk::Interpreter> interpreter_;
  std::deque<std::shared_ptr<RunningTask>> running_tasks_;
  std::deque<std::shared_ptr<RunningTask>> queued_tasks_;
  running_proc_id last_id_ = 1;
};

}  // namespace scheduler
}  // namespace donk

#endif  // __DONK_INTERPRETER_SCHEDULER_H__
