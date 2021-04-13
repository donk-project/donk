// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_PROJECT_DONK_CORE_ARGS_H__
#define __DONK_PROJECT_DONK_CORE_ARGS_H__

#include <initializer_list>
#include <string>

#include "donk/core/assoc_list.h"
#include "donk/core/vars.h"

namespace donk {

// proc_args_t is the representation of passed arguments to a DM proc.
// It is a thin wrapper over donk::assoc_list_t, with specific helpers
// and syntax to make it more useful.
class proc_args_t {
 public:
  proc_args_t() {}
  ~proc_args_t() {}

  // single string call = single string argument with no name
  proc_args_t(std::string);
  proc_args_t(std::initializer_list<int> l);
  proc_args_t(std::initializer_list<var_t> l);

  int size() const;
  bool empty() const;

  std::shared_ptr<var_t> pop_first();

  // coords maps three integers to the names "x", "y", and "z".
  static std::shared_ptr<proc_args_t> coords(int x, int y, int z);

  // return argument by name or position
  std::shared_ptr<var_t> v(const std::string&);
  std::shared_ptr<var_t> v(int);

 private:
  assoc_list_t args_;
};

}  // namespace donk

namespace fmt {

template <>
struct fmt::formatter<::donk::proc_args_t> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(::donk::proc_args_t const& p, FormatContext& ctx) {
    return fmt::format_to(ctx.out(), "proc_args_t<{}, {} elements>",
                          fmt::ptr(&p), p.size());
  }
};

}  // namespace fmt

#endif  // __DONK_PROJECT_DONK_CORE_ARGS_H__
