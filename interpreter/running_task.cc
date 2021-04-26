// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/interpreter/running_task.h"

namespace donk {
namespace scheduler {

std::shared_ptr<RunningTask> RunningTask::Spawn(transpiled_proc proc,
                                                proc_args_t& args,
                                                running_proc_id id) {
  auto task = std::make_shared<RunningTask>();
  task->name_ = "DONKAPI_SPAWN_PROC";
  task->proc_ = proc;
  task->args_ = args;
  task->proc_id_ = id;
  task->is_spawn_ = true;
  return task;
}

std::shared_ptr<RunningTask> RunningTask::Direct(std::shared_ptr<iota_t> src,
                                                 transpiled_proc proc,
                                                 std::string name,
                                                 proc_args_t& args,
                                                 running_proc_id id) {
  auto task = std::make_shared<RunningTask>();
  task->src_ = src;
  task->proc_ = proc;
  task->name_ = name;
  task->args_ = args;
  task->proc_id_ = id;
  return task;
}

std::shared_ptr<RunningTask> RunningTask::ObjCall(std::shared_ptr<iota_t> src,
                                                  std::string name,
                                                  proc_args_t args,
                                                  running_proc_id id) {
  auto task = std::make_shared<RunningTask>();
  task->src_ = src;
  task->name_ = name;
  task->args_ = args;
  task->proc_id_ = id;
  return task;
}

}  // namespace scheduler
}  // namespace donk
