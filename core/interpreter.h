// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_PROJECT_DONK_CORE_INTERPRETER_H__
#define __DONK_PROJECT_DONK_CORE_INTERPRETER_H__

#include <string>

#include "donk/core/path.h"
#include "donk/core/procs.h"

namespace donk {

class iota_t;
class preset_t;
class proc_args_t;
class var_t;

namespace ecs {
class EcsManager;
}  // namespace ecs

namespace mapping {
class MapRoster;
}  // namespace mapping

class Interpreter {
 public:
  // iota creation methods.
  template <typename Derived>
  std::shared_ptr<Derived> Make(std::string);

  template <typename Derived>
  std::shared_ptr<Derived> Make(preset_t);

  virtual std::shared_ptr<iota_t> MakeArbitrary(std::string) = 0;
  virtual running_proc_info& QueueSpawn(transpiled_proc spawn,
                                        proc_args_t& args) = 0;
  virtual running_proc_info& QueueProc(std::shared_ptr<iota_t> iota,
                                       std::string name, proc_args_t& args) = 0;
  virtual running_proc_info& QueueChild(std::shared_ptr<iota_t> iota,
                                        std::string name,
                                        proc_args_t& args) = 0;
  virtual running_proc_info& QueueChild(std::shared_ptr<iota_t> iota,
                                        transpiled_proc proc, std::string name,
                                        proc_args_t& args) = 0;

  virtual void RegisterCoreprocs() = 0;
  virtual void CreateWorld() = 0;
  virtual void SetRegistrationFunctions(
      std::shared_ptr<
          std::map<path_t, std::vector<std::function<void(iota_t&)>>>>) = 0;
  virtual std::shared_ptr<iota_t> world() = 0;
  virtual std::shared_ptr<donk::mapping::MapRoster> map() = 0;
  virtual std::shared_ptr<var_t> Corevar(std::string s) = 0;
  virtual std::shared_ptr<ecs::EcsManager> EcsManager() = 0;
  virtual std::shared_ptr<iota_t> Global() = 0;
  virtual void Stop() = 0;
  virtual bool Active() = 0;
};

}  // namespace donk

#endif  // __DONK_PROJECT_DONK_CORE_INTERPRETER_H__
