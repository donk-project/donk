// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/interpreter/scheduler.h"

#include <deque>
#include <functional>
#include <string>

#include "cppcoro/generator.hpp"
#include "cppcoro/sync_wait.hpp"
#include "cppcoro/task.hpp"
#include "donk/core/iota.h"
#include "donk/core/procs.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "spdlog/spdlog.h"

namespace donk {
namespace scheduler {

std::string Scheduler::DEBUG__OutputState() const {
  std::vector<std::string> output;

  output.push_back(fmt::format(
      "Scheduler last_tick={} last_id={} queued={} running={}", last_tick_,
      last_id_, queued_tasks_.size(), running_tasks_.size()));
  for (auto task_stack : running_tasks_) {
    output.push_back("Stack");
    for (auto rt : *task_stack) {
      output.push_back(fmt::format(" {} {}", rt->proc_id(), rt->name()));
    }
  }
  for (auto rt : queued_tasks_) {
    output.push_back(fmt::format("${} {}", rt->proc_id(), rt->name()));
  }

  return fmt::format("{}", fmt::join(output, "\n"));
}

running_proc_info& Scheduler::QueueProc(std::shared_ptr<iota_t> iota,
                                        std::string name, proc_args_t args) {
  last_id_++;
  running_proc_info info;
  info.callinstruct = proc_callinstruct::kNew;
  auto rt = RunningTask::ObjCall(iota, name, args, last_id_);
  rt->set_first_status(info);
  queued_tasks_.push_back(rt);
  return rt->first_status();
}

running_proc_info& Scheduler::QueueChild(std::shared_ptr<iota_t> iota,
                                         std::string name, proc_args_t args) {
  last_id_++;
  running_proc_info info;
  info.callinstruct = proc_callinstruct::kAwaitingChild;
  info.parent_task = running_task_;
  auto rt = RunningTask::ObjCall(iota, name, args, last_id_);
  rt->set_first_status(info);
  PrepQueuedTask(rt);
  running_stack_->Push(rt);
  return rt->first_status();
}

running_proc_info& Scheduler::QueueChild(std::shared_ptr<iota_t> iota,
                                         transpiled_proc proc, std::string name,
                                         proc_args_t args) {
  last_id_++;
  running_proc_info info;
  info.callinstruct = proc_callinstruct::kAwaitingChild;
  info.parent_task = running_task_;
  auto rt = RunningTask::Direct(iota, proc, name, args, last_id_);
  rt->set_first_status(info);
  PrepQueuedTask(rt);
  running_stack_->Push(rt);
  return rt->first_status();
}

running_proc_info& Scheduler::QueueSpawn(transpiled_proc spawn,
                                         proc_args_t& args) {
  last_id_++;
  running_proc_info info;
  info.callinstruct = proc_callinstruct::kSpawn;
  auto rt = RunningTask::Spawn(spawn, args, last_id_);
  rt->set_first_status(info);
  queued_tasks_.push_back(rt);
  return rt->first_status();
}

cppcoro::task<> Scheduler::Work(unsigned long last_tick) {
  // spdlog::info("Work tick {} queued_count={} running_count={}", last_tick,
  //              queued_tasks_.size(), running_tasks_.size());
  last_tick_ = last_tick;
  // We pop each stack off the front of the deque and put it at the back
  // when done with it, and we only do that as many times as there were
  // originally stacks in the deque.
  int len = running_tasks_.size();
  if (len > 0) {
    int i = 0;
    while (i < len) {
      auto ts = running_tasks_.front();
      running_tasks_.pop_front();

      // TODO: Kind of sloppy that the completed tasks have to be cleaned up
      // here
      if (ts->empty()) {
        i++;
        continue;
      }

      if (ts->LastInstruction() == proc_callinstruct::kSleepRequested) {
        if (ts->IsSleepElapsed(last_tick)) {
          ProcessRunningTask(ts, last_tick);
        }
      } else {
        ProcessRunningTask(ts, last_tick);
      }

      if (!ts->empty()) {
        running_tasks_.push_back(ts);
      }

      i++;
    }
  }

  if (queued_tasks_.empty()) {
    co_return;
  }

  while (!queued_tasks_.empty()) {
    // Check for un-running, queued tasks, and add them to list of task stacks.
    auto rt = queued_tasks_.front();
    queued_tasks_.pop_front();
    PrepQueuedTask(rt);
    auto new_stack_ = std::make_shared<TaskStack>();
    new_stack_->Push(rt);
    running_tasks_.push_back(new_stack_);

    ProcessRunningTask(new_stack_, last_tick);
  }
}

void Scheduler::PrepQueuedTask(std::shared_ptr<RunningTask> rt) {
  if (rt->is_spawn()) {
    auto ctxt = std::make_shared<proc_ctxt_t>(interpreter_);
    rt->set_context(ctxt);
    rt->set_current_tick(last_tick_);
    rt->Await();
  } else {
    if (!rt->HasCallable()) {
      auto func = rt->src()->proc_table().GetProcByName(rt->name());
      auto proc = func.GetInternalProc();
      rt->set_proc(proc);
    }

    // TODO: Fix assignment of src and usr
    auto ctxt = std::make_shared<proc_ctxt_t>(interpreter_);
    ctxt->SetSrc(rt->src());
    ctxt->SetUsr(rt->src());

    rt->set_context(ctxt);
    rt->set_current_tick(last_tick_);
    rt->Await();
  }
}

void Scheduler::ProcessRunningTask(std::shared_ptr<TaskStack> ts,
                                   unsigned long last_tick) {
  running_stack_ = ts;
  auto rt = running_stack_->front();
  running_task_ = rt;
  // spdlog::info("ProcessRunningTask({}), stack size={}", rt->name(),
  // ts->size());
  for (auto result : rt->task()) {
    rt->set_last_status(result);
    if (result.callinstruct == proc_callinstruct::kSleepRequested) {
      rt->set_sleep_ticks_requested(result.requested_tick_delay);
      rt->set_current_tick(last_tick);
      return;
    } else if (result.callinstruct == proc_callinstruct::kAwaitingChild) {
      ProcessRunningTask(ts, last_tick);
      return;
    }
  }

  if (rt->first_status().parent_task != nullptr) {
    // Hand the return value back to the parent task.
    if (rt->context()->GetResult() != nullptr) {
      rt->first_status().parent_task->set_subtask_result(
          rt->context()->GetResult());
    }
    ts->pop_front();
    // Ending a child task isn't a stopping point for the scheduler.
    ProcessRunningTask(ts, last_tick);
    return;
  }

  ts->pop_front();
}

}  // namespace scheduler
}  // namespace donk
