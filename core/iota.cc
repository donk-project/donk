// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/core/iota.h"

#include <functional>
#include <stdexcept>

#include "cppcoro/sync_wait.hpp"
#include "donk/core/path.h"
#include "donk/core/preset.h"

namespace donk {

iota_t::iota_t(donk::path_t path) : path_(path), proc_table_(), var_table_() {}

void iota_t::RegisterProc(std::string s, transpiled_proc func) {
  proc_table_.Register(s, func);
}

void iota_t::RegisterVar(const std::string& s, var_t& t) {
  var_table_.Register(s, t);
}

const path_t iota_t::ParentType() const {
  path_t matcher(GetPath());
  while (!matcher.IsRoot()) {
    if (matcher.IsCoretype()) {
      return matcher;
    }
  }
  throw std::runtime_error("all iotas must have a DM type-parent");
}

void iota_t::ProcSettings(std::string s, proc_settings_t t) {
  proc_table_.ApplySettings(s, t);
}

void iota_t::RegisterVar(const std::string& s) { var_table_.Register(s); }

std::shared_ptr<var_t> iota_t::v(const std::string& s) {
  return var_table_.Lookup(s);
}

const path_t iota_t::GetPath() const { return path_; }

void iota_t::Apply(preset_t preset) {
  entity_id_ = preset.entity_id();
  var_table_.RegisterPreset(preset);
}

void iota_t::InternalCoreRegister() {}

void iota_t::ProcInput(std::string s, proc_input_t i) {
  proc_table_.AddInput(s, i);
}

}  // namespace donk
