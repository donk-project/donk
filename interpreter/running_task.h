// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_PROJECT_DONK_INTERPRETER_RUNNING_TASK_H__
#define __DONK_PROJECT_DONK_INTERPRETER_RUNNING_TASK_H__

#include <string>

#include "donk/core/iota.h"
#include "donk/core/procs.h"

namespace donk {
namespace scheduler {

// RunningTask is a representation of an executing proc.
class RunningTask {
 public:
  // Provides a running task from an anonymous spawn block, args, and process
  // ID.
  static std::shared_ptr<RunningTask> Spawn(transpiled_proc spawned_proc,
                                            proc_args_t& args,
                                            running_proc_id id);

  // Provides a running task from a given src object and arbitrary proc, with
  // process name, args, and process ID.
  static std::shared_ptr<RunningTask> Direct(std::shared_ptr<iota_t> src,
                                             transpiled_proc proc,
                                             std::string name,
                                             proc_args_t& args,
                                             running_proc_id id);

  // Provides a running task from a given src object and a name of one of its
  // procs, args, and process ID.
  static std::shared_ptr<RunningTask> ObjCall(std::shared_ptr<iota_t> src,
                                              std::string name, proc_args_t a,
                                              running_proc_id id);

  std::string name() const { return name_; }

  running_proc_id proc_id() const { return proc_id_; }

  running_proc_info& first_status() { return first_status_; }
  running_proc_info& last_status() { return last_status_; }

  void set_first_status(running_proc_info first_status) {
    first_status_ = first_status;
  }

  void set_context(std::shared_ptr<proc_ctxt_t> context) { context_ = context; }

  void set_current_tick(int current_tick) { current_tick_ = current_tick; }

  transpiled_proc& proc() { return proc_; }

  void Await() {
    auto awaiter = proc_(*context_, args_);
    task_ = std::move(awaiter);
  }

  std::shared_ptr<iota_t> src() { return src_; }

  bool HasCallable() const { return (bool)proc_; }

  bool is_spawn() const { return is_spawn_; }

  void set_proc(transpiled_proc proc) { proc_ = proc; }

  running_proc& task() { return task_; }

  void set_last_status(running_proc_info last_status) {
    last_status_ = last_status;
  }

  void set_sleep_ticks_requested(int sleep_ticks_requested) {
    sleep_ticks_requested_ = sleep_ticks_requested;
  }

  std::shared_ptr<proc_ctxt_t> context() { return context_; }

  int current_tick() const { return current_tick_; }

  int sleep_ticks_requested() const { return sleep_ticks_requested_; }

  void set_subtask_result(std::shared_ptr<var_t> subtask_result) {
    subtask_result_ = subtask_result;
  }

 private:
  // TODO: This is a ton of crap to store for every individual running task
  std::shared_ptr<iota_t> src_;
  std::string name_;
  proc_args_t args_;
  int sleep_ticks_requested_;
  int current_tick_;
  running_proc task_;
  transpiled_proc proc_;
  running_proc_id proc_id_;
  std::shared_ptr<proc_ctxt_t> context_;
  running_proc_info first_status_;
  // The last value yielded from this proc's coroutine.
  running_proc_info last_status_;
  std::shared_ptr<var_t> subtask_result_;
  bool is_spawn_;
};

}  // namespace scheduler
}  // namespace donk

#endif  // __DONK_PROJECT_DONK_INTERPRETER_RUNNING_TASK_H__
