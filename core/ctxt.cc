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

std::shared_ptr<iota_t> proc_ctxt_t::make(std::string str) {
  return interpreter_->MakeArbitrary(str);
}

void proc_ctxt_t::AssignSrc(std::shared_ptr<iota_t> i) { src_ = i; }

void proc_ctxt_t::AssignUsr(std::shared_ptr<iota_t> i) { usr_ = i; }

void proc_ctxt_t::Result(std::shared_ptr<var_t> v) { result_ = v; }

void proc_ctxt_t::Result(int i) { result_ = std::make_shared<var_t>(i); }

void proc_ctxt_t::Result(std::shared_ptr<iota_t> i) { result_->data_ = i; }

std::shared_ptr<var_t> proc_ctxt_t::cvar(std::string s) {
  return interpreter_->Corevar(s);
}

std::shared_ptr<donk::mapping::MapRoster> proc_ctxt_t::map() {
  return interpreter_->map();
}

std::shared_ptr<donk::ecs::EcsManager> proc_ctxt_t::ecs() {
  return interpreter_->EcsManager();
}

running_proc_info proc_ctxt_t::sleep(int ticks) {
  running_proc_info info;
  info.callinstruct = proc_callinstruct::kSleepRequested;
  info.requested_tick_delay = ticks;
  return info;
}

running_proc_info proc_ctxt_t::Proc(std::shared_ptr<iota_t> iota,
                                    std::string proc_name, proc_args_t args) {
  auto pid = interpreter_->QueueProc(iota, proc_name, args);
  running_proc_info info;
  info.proc = pid;
  info.callinstruct = proc_callinstruct::kNew;
  return info;
}

running_proc_info proc_ctxt_t::ChildProc(std::shared_ptr<iota_t> iota,
                                         std::string proc_name,
                                         proc_args_t args) {
  auto pid = interpreter_->QueueProc(iota, proc_name, args);
  running_proc_info info;
  info.callinstruct = proc_callinstruct::kAwaitingChild;
  info.childproc = pid;
  return info;
}

std::shared_ptr<var_t> proc_ctxt_t::SProc(std::shared_ptr<iota_t> iota,
                                          std::string proc_name,
                                          proc_args_t args) {
  return interpreter_->RunProcNow(iota, proc_name, args);
}

running_proc_info proc_ctxt_t::Unimplemented(std::string proc_name) {
  spdlog::critical("Unimplemented proc {} was called!", proc_name);
  running_proc_info info;
  info.callinstruct = proc_callinstruct::kUnimplemented;
  return info;
}

void proc_ctxt_t::Broadcast(std::shared_ptr<var_t> receiver_list,
                            std::shared_ptr<var_t> obj) {}

running_proc_info proc_ctxt_t::Spawn(transpiled_proc spawn) {}

std::shared_ptr<iota_t> proc_ctxt_t::Global() { return interpreter_->Global(); }

}  // namespace donk
