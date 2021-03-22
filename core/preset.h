// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_CORE_PRESET_H__
#define __DONK_CORE_PRESET_H__

#include "donk/core/assoc_list.h"
#include "donk/core/vars.h"

namespace donk {

// A preset is a path and set of variables used to instantiate an iota.
// A uuid can be provided in certain situations to sync an object with
// its uuid on the wire.
class preset_t {
 public:
  preset_t(path_t path) : path_(path), uuid_(0) {}
  preset_t(const preset_t& other)
      : path_(other.path_), uuid_(other.uuid_), vars_(other.vars_) {}

  const path_t& path() const { return path_; }

  uuid_t uuid() const { return uuid_; }
  void uuid(uuid_t uuid) { uuid_ = uuid; }
  assoc_list_t& vars() { return vars_; }

 private:
  path_t path_;
  uuid_t uuid_;
  assoc_list_t vars_;

  friend class var_table_t;
};

}  // namespace donk

namespace fmt {

template <>
struct fmt::formatter<donk::preset_t> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(donk::preset_t const& p, FormatContext& ctx) {
    return fmt::format_to(ctx.out(), "preset_t<{}>", p.path().fq());
  }
};

}  // namespace fmt

#endif  // __DONK_CORE_PRESET_H__
