// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __SNOWFROST_DONK_CORE_PROCS_H__
#define __SNOWFROST_DONK_CORE_PROCS_H__

#include <deque>
#include <exception>
#include <functional>
#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "cppcoro/generator.hpp"
#include "donk/core/assoc_list.h"
#include "donk/core/path.h"
#include "donk/core/vars.h"
#include "fmt/format.h"
#include "spdlog/spdlog.h"

namespace donk {
namespace internal {
class Interpreter;
}

// proc_callinstruct is the list of available instruction codes a caller
// can receive from a running proc.
enum proc_callinstruct {
  kNew = 0,
  kSleepRequested,
  kAwaitingChild,
  kComplete,
  kFailure,
  kUnimplemented,
};

using running_proc_id = unsigned int;

// running_proc_info encapsulates the state that a proc may wish to relay to its
// caller or other procs waiting on its completion.
struct running_proc_info {
  proc_callinstruct callinstruct;
  int requested_tick_delay;
  running_proc_id proc;
  running_proc_id childproc;
};

// running_proc is the universal return type of all procs.
using running_proc = cppcoro::generator<running_proc_info>;

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

class iota_t;
class Interpreter;

namespace ecs {
class EcsManager;
}  // namespace ecs

namespace mapping {
class MapRoster;
}  // namespace mapping

class proc_ctxt_t;
class proc_args_t;

// transpiled_proc is the universal signature of all transpiled DM procs.
using transpiled_proc = std::function<running_proc(proc_ctxt_t&, proc_args_t&)>;

// proc_ctxt_t is the global state provided to a proc during execution.
// It also consists of various helper methods used in codegen to make
// the generated code cleaner.
class proc_ctxt_t {
 public:
  proc_ctxt_t(std::shared_ptr<donk::Interpreter>);

  proc_ctxt_t() = delete;

  std::shared_ptr<iota_t> usr();
  std::shared_ptr<iota_t> src();
  std::shared_ptr<iota_t> world();
  std::shared_ptr<var_t> result();

  void del(std::shared_ptr<iota_t>) {
    // TODO: Destroy entt entities here most likely
  }

  std::shared_ptr<iota_t> make(std::string);

  running_proc_info Proc(std::shared_ptr<iota_t> iota, std::string proc_name,
                         proc_args_t args);

  running_proc_info Spawn(transpiled_proc spawn);

  running_proc_info ChildProc(std::shared_ptr<iota_t> iota,
                              std::string proc_name, proc_args_t args);

  // If a proc is known to not need asynchronous invocation,
  // SProc continuously yields the proc until a result is found.
  std::shared_ptr<var_t> SProc(std::shared_ptr<iota_t> iota,
                               std::string proc_name, proc_args_t args);

  // Global vars
  std::shared_ptr<var_t> cvar(std::string);

  running_proc_info sleep(int ticks);
  std::shared_ptr<var_t> ChildResult() { return child_result_; }
  running_proc_info Unimplemented(std::string proc_name);

  std::shared_ptr<donk::ecs::EcsManager> ecs();
  std::shared_ptr<donk::mapping::MapRoster> map();
  std::shared_ptr<iota_t> Global();

  void Broadcast(std::shared_ptr<var_t> receiver_list,
                 std::shared_ptr<var_t> obj);

  void AssignSrc(std::shared_ptr<iota_t> i);
  void AssignUsr(std::shared_ptr<iota_t> i);
  void ChildResult(std::shared_ptr<var_t> v) { child_result_ = v; }
  void Result(std::shared_ptr<var_t>);
  void Result(int);
  void Result(std::shared_ptr<iota_t>);

 private:
  std::string name_;
  std::shared_ptr<var_t> result_;
  std::shared_ptr<var_t> child_result_;
  std::shared_ptr<donk::Interpreter> interpreter_;
  std::shared_ptr<iota_t> usr_;
  std::shared_ptr<iota_t> src_;

  friend class iota_t;
  friend class var_t;
  friend class proc_table_t;
};

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

class proc_inputs_t {
 public:
  bool text;
  bool password;
  bool message;
  bool command_text;
  bool num;
  bool icon;
  bool sound;
  bool file;
  bool key;
  bool color;
  bool null;

  bool atom;
  bool turf;
  bool obj;
  bool mob;
  bool anything;
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
  proc_inputs_t inputs_;

  friend class proc_table_t;
};

class proc_table_t {
 public:
  proc_table_t() : procs_() {}

  void Register(std::string s, transpiled_proc func);
  void ApplySettings(std::string s, proc_settings_t settings);

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

template <>
struct fmt::formatter<::donk::proc_args_t> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(::donk::proc_args_t const& p, FormatContext& ctx) {
    return fmt::format_to(ctx.out(), "proc_args_t<{}>", fmt::ptr(&p));
  }
};

}  // namespace fmt

#endif  // __SNOWFROST_DONK_CORE_PROCS_H__
