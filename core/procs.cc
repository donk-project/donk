// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/core/procs.h"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "donk/core/interpreter.h"
#include "donk/core/iota.h"
#include "donk/core/vars.h"
#include "fmt/format.h"

namespace donk {

void proc_table_t::Register(std::string s, transpiled_proc func) {
  for (auto& result : procs_) {
    if (result.first == s) {
      result.second.insert(result.second.begin(), proc_t(func));
    }
  }
  procs_[s].push_back(proc_t(func));
}

void proc_table_t::ApplySettings(std::string s, proc_settings_t settings) {
  spdlog::warn("ApplySettings not implemented");
}

void proc_table_t::AddInput(std::string s, proc_input_t input) {
  spdlog::warn("AddInput not implemented");
}

}  // namespace donk
