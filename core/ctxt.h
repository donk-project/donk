// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_PROJECT_DONK_CORE_CTXT_H__
#define __DONK_PROJECT_DONK_CORE_CTXT_H__

#include <functional>

#include "donk/core/generator.h"
#include "donk/core/vars.h"

namespace donk {
namespace internal {
class Interpreter;
}

// proc_callinstruct is the list of available instruction codes a caller
// can receive from a running proc.
enum proc_callinstruct {
  kUnknownCallInstruct = 0,
  kNew,
  kSleepRequested,
  kAwaitingChild,
  kComplete,
  kFailure,
  kUnimplemented,
  kSpawn,
};

using running_proc_id = long;

namespace scheduler {
class RunningTask;
}

// running_proc_info encapsulates the state that a proc may wish to relay to its
// caller or other procs waiting on its completion.
struct running_proc_info {
  proc_callinstruct callinstruct = proc_callinstruct::kUnknownCallInstruct;
  int requested_tick_delay = -1;
  running_proc_id proc = -1;
  std::shared_ptr<scheduler::RunningTask> parent_task;
};

// running_proc is the universal return type of all procs.
using running_proc = generator<running_proc_info>;

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

  // Methods used in codegen
  std::shared_ptr<iota_t> usr();
  std::shared_ptr<iota_t> src();
  std::shared_ptr<iota_t> world();
  std::shared_ptr<var_t> result();

  void del(std::shared_ptr<iota_t>);

  std::shared_ptr<iota_t> make(std::string);

  // Gproc runs a core API call and returns the results immediately.
  std::shared_ptr<var_t> Gproc(std::string proc_name, proc_args_t args);

  // Methods returning running_proc_info must be co_yielded or co_returned.

  // Spawn tells the interpreter to create a new task stack for the provided
  // function/lambda.
  running_proc_info& Spawn(transpiled_proc spawn, proc_args_t& args,
                           int tick_delay = 0);

  // ChildProc tells the interpreter to push the requested proc onto the
  // current task stack. This will pause the current proc and execute the
  // requested proc.
  running_proc_info& ChildProc(std::shared_ptr<iota_t> iota,
                               std::string proc_name, proc_args_t args);

  // DirectProc allows a proc to be run against an arbitrary iota.
  // Typically used for parent calls.
  running_proc_info& DirectProc(std::shared_ptr<iota_t> iota,
                                transpiled_proc proc, std::string name,
                                proc_args_t args);

  // Sleep tells the interpreter to sleep the proc for the provided number
  // of ticks.
  running_proc_info Sleep(int ticks);

  // Unimplemented is used in codegen of BYOND API stubs.
  running_proc_info Unimplemented(std::string proc_name);

  // Gvar returns a global variable with the provided name.
  std::shared_ptr<var_t> Gvar(std::string);

  void Broadcast(std::shared_ptr<var_t> receiver_list,
                 std::shared_ptr<var_t> obj);

  // Global returns the root object, `/`.
  std::shared_ptr<iota_t> Global();

  // Shutdown tells the interpreter to exit.
  void Shutdown();

  // Methods used in the API outside codegen, by and large
  std::shared_ptr<var_t> GetResult();
  std::shared_ptr<donk::ecs::EcsManager> GetEcsManager();
  std::shared_ptr<donk::mapping::MapRoster> GetMapRoster();

  void SetSrc(std::shared_ptr<iota_t> i);
  void SetUsr(std::shared_ptr<iota_t> i);
  void SetResult(var_t);
  void SetResult(std::shared_ptr<var_t>);
  void SetResult(int);
  void SetResult(std::shared_ptr<iota_t>);

 private:
  std::string name_;
  std::shared_ptr<var_t> result_;
  std::shared_ptr<donk::Interpreter> interpreter_;
  std::shared_ptr<iota_t> usr_;
  std::shared_ptr<iota_t> src_;

  friend class iota_t;
  friend class var_t;
  friend class proc_table_t;
};

}  // namespace donk

namespace fmt {

template <>
struct fmt::formatter<donk::running_proc_info> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(donk::running_proc_info const& rpi, FormatContext& ctx) {
    return fmt::format_to(
        ctx.out(), "running_proc_info<callinstruct={} tick_delay={} proc={}>",
        rpi.callinstruct, rpi.requested_tick_delay, rpi.proc);
  }
};

}  // namespace fmt

#endif  // __DONK_PROJECT_DONK_CORE_CTXT_H__
