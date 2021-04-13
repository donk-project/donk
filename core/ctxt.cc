// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/core/interpreter.h"
#include "donk/core/iota.h"
#include "donk/core/procs.h"
#include "donk/core/vars.h"

namespace donk {

proc_ctxt_t::proc_ctxt_t(std::shared_ptr<donk::Interpreter> interpreter)
    : result_(std::make_shared<var_t>()), interpreter_(interpreter) {}

std::shared_ptr<iota_t> proc_ctxt_t::usr() { return usr_; }

std::shared_ptr<iota_t> proc_ctxt_t::src() { return src_; }

std::shared_ptr<iota_t> proc_ctxt_t::world() { return interpreter_->world(); }

std::shared_ptr<var_t> proc_ctxt_t::result() { return result_; }

void proc_ctxt_t::del(std::shared_ptr<iota_t> d) {}

std::shared_ptr<iota_t> proc_ctxt_t::make(std::string str) {
  return interpreter_->MakeArbitrary(str);
}

std::shared_ptr<var_t> proc_ctxt_t::Gproc(std::string proc_name,
                                          proc_args_t args) {
  auto ctxt = std::make_shared<proc_ctxt_t>(interpreter_);
  auto proc = Global()->proc_table().GetProcByName(proc_name);
  auto func = proc.GetInternalFunc();
  auto generator = func(*ctxt, args);
  for (auto t : generator) {
    // If a proc called here requests a sleep or spawn it will be ignored
    // because only the scheduler responds to these instructions, and Gproc
    // skips the scheduler altogether.
    if (t.callinstruct == proc_callinstruct::kSleepRequested) {
      spdlog::critical("Gproc {} called sleep, which is ignored", proc_name);
    } else if (t.callinstruct == proc_callinstruct::kAwaitingChild) {
      spdlog::critical("Gproc {} awaiting child, which is ignored", proc_name);
    }
  }
  return ctxt->result();
}

running_proc_info& proc_ctxt_t::Spawn(transpiled_proc spawn, proc_args_t& args,
                                      int tick_delay) {
  return interpreter_->QueueSpawn(spawn, args);
}

running_proc_info& proc_ctxt_t::ChildProc(std::shared_ptr<iota_t> iota,
                                          std::string proc_name,
                                          proc_args_t args) {
  return interpreter_->QueueChild(iota, proc_name, args);
}

running_proc_info proc_ctxt_t::Sleep(int ticks) {
  running_proc_info info;
  info.callinstruct = proc_callinstruct::kSleepRequested;
  info.requested_tick_delay = ticks;
  return info;
}

running_proc_info proc_ctxt_t::Unimplemented(std::string proc_name) {
  spdlog::critical("Unimplemented proc {} was called!", proc_name);
  running_proc_info info;
  info.callinstruct = proc_callinstruct::kUnimplemented;
  return info;
}

std::shared_ptr<var_t> proc_ctxt_t::Gvar(std::string s) {
  return interpreter_->Corevar(s);
}

void proc_ctxt_t::Broadcast(std::shared_ptr<var_t> receiver_list,
                            std::shared_ptr<var_t> obj) {}

std::shared_ptr<iota_t> proc_ctxt_t::Global() { return interpreter_->Global(); }

void proc_ctxt_t::Shutdown() { interpreter_->Stop(); }

std::shared_ptr<var_t> proc_ctxt_t::GetResult() { return result_; }

std::shared_ptr<donk::ecs::EcsManager> proc_ctxt_t::GetEcsManager() {
  return interpreter_->EcsManager();
}

std::shared_ptr<donk::mapping::MapRoster> proc_ctxt_t::GetMapRoster() {
  return interpreter_->map();
}

void proc_ctxt_t::SetSrc(std::shared_ptr<iota_t> i) { src_ = i; }

void proc_ctxt_t::SetUsr(std::shared_ptr<iota_t> i) { usr_ = i; }

void proc_ctxt_t::SetResult(std::shared_ptr<var_t> v) { result_ = v; }

void proc_ctxt_t::SetResult(int i) { result_ = std::make_shared<var_t>(i); }

void proc_ctxt_t::SetResult(std::shared_ptr<iota_t> i) { result_->data_ = i; }

void proc_ctxt_t::SetResult(var_t v) { *result_ = v; };

}  // namespace donk
