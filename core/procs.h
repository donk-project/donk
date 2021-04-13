// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __SNOWFROST_DONK_CORE_PROCS_H__
#define __SNOWFROST_DONK_CORE_PROCS_H__

#include <deque>
#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "cppcoro/generator.hpp"
#include "donk/core/args.h"
#include "donk/core/assoc_list.h"
#include "donk/core/ctxt.h"
#include "donk/core/path.h"
#include "donk/core/vars.h"
#include "fmt/format.h"
#include "spdlog/spdlog.h"

namespace donk {

enum proc_access {
  kUnknown = 0,
  kInView,
  kInOView,
  kInUsrLoc,
  kInUsr,
  kInWorld,
  kEqUsr,
  kInGroup
};

class proc_input_t {
 public:
  proc_input_t(std::string name) : name_(name) {}
  std::string name_;

  proc_input_t& text(bool v) {
    text_ = v;
    return *this;
  }

  proc_input_t& password(bool v) {
    password_ = v;
    return *this;
  }

  proc_input_t& message(bool v) {
    message_ = v;
    return *this;
  }

  proc_input_t& command_text(bool v) {
    command_text_ = v;
    return *this;
  }

  proc_input_t& num(bool v) {
    num_ = v;
    return *this;
  }

  proc_input_t& icon(bool v) {
    icon_ = v;
    return *this;
  }

  proc_input_t& sound(bool v) {
    sound_ = v;
    return *this;
  }

  proc_input_t& file(bool v) {
    file_ = v;
    return *this;
  }

  proc_input_t& key(bool v) {
    key_ = v;
    return *this;
  }

  proc_input_t& color(bool v) {
    color_ = v;
    return *this;
  }

  proc_input_t& null(bool v) {
    null_ = v;
    return *this;
  }

  proc_input_t& atom(bool v) {
    atom_ = v;
    return *this;
  }

  proc_input_t& turf(bool v) {
    turf_ = v;
    return *this;
  }

  proc_input_t& obj(bool v) {
    obj_ = v;
    return *this;
  }

  proc_input_t& mob(bool v) {
    mob_ = v;
    return *this;
  }

  proc_input_t& anything(bool v) {
    anything_ = v;
    return *this;
  }

  bool text_;
  bool password_;
  bool message_;
  bool command_text_;
  bool num_;
  bool icon_;
  bool sound_;
  bool file_;
  bool key_;
  bool color_;
  bool null_;
  bool atom_;
  bool turf_;
  bool obj_;
  bool mob_;
  bool anything_;
};

class proc_settings_t {
 public:
  proc_settings_t() {}

  proc_settings_t& name(std::string name) {
    name_ = name;
    return *this;
  }

  proc_settings_t& desc(std::string desc) {
    desc_ = desc;
    return *this;
  }

  proc_settings_t& category(std::string category) {
    category_ = category;
    return *this;
  }

  proc_settings_t& hidden(bool hidden) {
    hidden_ = hidden;
    return *this;
  }

  proc_settings_t& popup_menu(bool popup_menu) {
    popup_menu_ = popup_menu;
    return *this;
  }

  proc_settings_t& instant(bool instant) {
    instant_ = instant;
    return *this;
  }

  proc_settings_t& invisibility(int invisibility) {
    invisibility_ = invisibility;
    return *this;
  }

  proc_settings_t& access(proc_access access) {
    access_ = access;
    return *this;
  }

  proc_settings_t& range(int range) {
    range_ = range;
    return *this;
  }

  proc_settings_t& background(bool background) {
    background_ = background;
    return *this;
  }

  proc_settings_t& waitfor(bool waitfor) {
    waitfor_ = waitfor;
    return *this;
  }

 private:
  std::string name_;
  std::string desc_;
  std::string category_;
  bool hidden_ = false;
  bool popup_menu_ = true;
  bool instant_ = false;
  int invisibility_;
  proc_access access_;
  int range_ = 0;
  bool background_ = false;
  bool waitfor_ = true;
};

// proc_t wraps transpiled procs and provides settings metadata.
class proc_t {
 public:
  proc_t(transpiled_proc func) : func_(func) {}

  transpiled_proc GetInternalFunc() { return func_; }

 private:
  void Call(proc_ctxt_t&, proc_args_t&);
  transpiled_proc func_;
  proc_settings_t settings_;
  std::vector<proc_input_t> inputs_;

  friend class proc_table_t;
};

class proc_table_t {
 public:
  proc_table_t() : procs_() {}

  void Register(std::string s, transpiled_proc func);
  void ApplySettings(std::string s, proc_settings_t settings);
  void AddInput(std::string s, proc_input_t input);

  int size() const { return procs_.size(); }

  proc_t& GetProcByName(std::string s) {
    auto search = procs_.find(s);
    if (search != procs_.end()) {
      return search->second[0];
    } else {
      throw std::runtime_error(
          fmt::format("asked iota for unknown proc {}", s));
    }
  }

  bool HasProc(std::string name) const {
    auto search = procs_.find(name);
    return search != procs_.end();
  }

 public:
  std::map<std::string, std::vector<proc_t>> procs_;
};

}  // namespace donk

namespace fmt {

template <>
struct fmt::formatter<::donk::proc_t> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(::donk::proc_t const&, FormatContext& ctx) {
    return fmt::format_to(ctx.out(), "proc<>");
  }
};

template <>
struct fmt::formatter<::donk::proc_table_t> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(::donk::proc_table_t const& p, FormatContext& ctx) {
    fmt::format_to(ctx.out(), "proc_table<{}>", p.procs_.size());
    return ctx.out();
  }
};

template <>
struct fmt::formatter<::donk::proc_ctxt_t> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(::donk::proc_ctxt_t const&, FormatContext& ctx) {
    return fmt::format_to(ctx.out(), "proc_ctxt_t<>");
  }
};

}  // namespace fmt

#endif  // __SNOWFROST_DONK_CORE_PROCS_H__
