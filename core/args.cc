// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include <initializer_list>
#include <string>

#include "donk/core/procs.h"
#include "donk/core/vars.h"
#include "spdlog/spdlog.h"

namespace donk {

proc_args_t::proc_args_t(std::string s) {
  args_.insert(std::make_shared<var_t>(s));
}

std::shared_ptr<var_t> proc_args_t::pop_first() {
  auto x = args_.front();
  args_.pop_front();
  return x;
}

proc_args_t::proc_args_t(std::initializer_list<int> l) {
  for (auto& i : l) {
    args_.insert(std::make_shared<var_t>(i));
  }
}

proc_args_t::proc_args_t(std::initializer_list<var_t> l) {
  for (auto& i : l) {
    args_.insert(std::make_shared<var_t>(i));
  }
}

// Convenience method for calls that support args (X,Y,Z)
std::shared_ptr<proc_args_t> coords(int x, int y, int z) {
  auto coords = std::make_shared<proc_args_t>();
  *coords->v("x") = x;
  *coords->v("y") = y;
  *coords->v("z") = z;
  spdlog::info("coords: {} {} {}", x, y, z);
  return coords;
}

int proc_args_t::size() const { return args_.size(); }

bool proc_args_t::empty() const { return args_.empty(); }

std::shared_ptr<var_t> proc_args_t::v(const std::string& s) {
  return args_.v(s);
}

std::shared_ptr<var_t> proc_args_t::v(int i) { return args_.v(i); }

}  // namespace donk
