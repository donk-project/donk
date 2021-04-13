// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_INTERPRETER_RUNNER_RUNNER_H__
#define __DONK_INTERPRETER_RUNNER_RUNNER_H__

#include <functional>
#include <map>
#include <vector>

#include "donk/api/root.h"
#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/interpreter/interpreter.h"
#include "spdlog/spdlog.h"
#include "type_registrar.h"

// Runner is an executable which loads a Donk C++ library, attempts to find a
// proc /main() in the object root, and executes it. This is meant to be a sort
// of "headless" BYOND interpreter to test simple scripts.

namespace donk {
namespace runner {

class Runner {
 public:
  Runner() {
    auto collector = std::make_shared<std::map<
        donk::path_t, std::vector<std::function<void(donk::iota_t&)>>>>();
    dtpo::RegisterAll(collector);
    (*collector)[donk::path_t("/")].push_back(donk::api::Register);

    interpreter_ = donk::internal::Interpreter::Create();
    interpreter_->SetRegistrationFunctions(collector);
    interpreter_->RegisterCoreprocs();
    interpreter_->CreateWorld();
  }

  void CallMain() {
    auto args = proc_args_t();
    interpreter_->QueueProc(interpreter_->Global(), "main", args);
    interpreter_->ExitOnEmptyQueue();
    interpreter_->Run();
    while (interpreter_->Active()) {
      interpreter_->Tick();
      while (!interpreter_->GetWorld()->internal_broadcast_log_.empty()) {
        auto s = interpreter_->GetWorld()->PopBroadcast();
        spdlog::info("{}", s);
      }
    }
  }

 private:
  std::shared_ptr<donk::internal::Interpreter> interpreter_;
};

}  // namespace runner
}  // namespace donk

#endif  // __DONK_INTERPRETER_RUNNER_RUNNER_H__
