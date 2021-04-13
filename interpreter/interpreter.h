// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_INTERPRETER_INTERPRETER_H__
#define __DONK_INTERPRETER_INTERPRETER_H__

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "cppcoro/sync_wait.hpp"
#include "donk/api/client.h"
#include "donk/api/database.h"
#include "donk/api/database/query.h"
#include "donk/api/datum.h"
#include "donk/api/datum/atom.h"
#include "donk/api/datum/atom/area.h"
#include "donk/api/datum/atom/movable.h"
#include "donk/api/datum/atom/movable/mob.h"
#include "donk/api/datum/atom/movable/obj.h"
#include "donk/api/datum/atom/turf.h"
#include "donk/api/exception.h"
#include "donk/api/icon.h"
#include "donk/api/image.h"
#include "donk/api/list.h"
#include "donk/api/matrix.h"
#include "donk/api/mutable_appearance.h"
#include "donk/api/regex.h"
#include "donk/api/root.h"
#include "donk/api/savefile.h"
#include "donk/api/sound.h"
#include "donk/api/world.h"
#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"
#include "donk/core/vars.h"
#include "donk/interpreter/ecs_manager.h"
#include "donk/interpreter/importer.h"
#include "donk/interpreter/scheduler.h"
#include "donk/interpreter/timekeeper.h"
#include "donk/mapping/map_view.h"
#include "fmt/format.h"
#include "spdlog/spdlog.h"

namespace donk {
class proc_args_t;

namespace mapping {
class MapRoster;
}  // namespace mapping

namespace api {
namespace world {
class world_coretype;
}  // namespace world
}  // namespace api

namespace internal {

class Interpreter : public std::enable_shared_from_this<Interpreter>,
                    public donk::Interpreter {
 public:
  template <typename... T>
  static std::shared_ptr<donk::internal::Interpreter> Create(T&&... t) {
    auto interpreter = std::shared_ptr<donk::internal::Interpreter>(
        new donk::internal::Interpreter(std::forward<T>(t)...));
    interpreter->scheduler_ =
        std::make_shared<donk::scheduler::Scheduler>(interpreter);
    return interpreter;
  }

  virtual ~Interpreter() {}

  std::shared_ptr<donk::resources::Importer> GetImporter() { return importer_; }
  void Import(std::string root_filename);
  void ResetMapRoster(donk::mapping::MapRoster& roster);
  void ResetMaps();
  void UpdateUuidLinks(std::map<donk::entity_id, std::vector<std::string>>&
                           update_entity_id_varnames);

  // Differs from world() in that we are returning the known coretype.
  std::shared_ptr<donk::api::world::world_coretype> GetWorld() {
    return world_;
  }

  std::shared_ptr<donk::mapping::MapRoster> map() override;
  std::shared_ptr<iota_t> MakeArbitrary(std::string) override;
  std::shared_ptr<iota_t> world() override;
  std::shared_ptr<var_t> Corevar(std::string s) override;
  void CreateWorld() override;
  void RegisterCoreprocs() override;
  void SetRegistrationFunctions(
      std::shared_ptr<
          std::map<path_t, std::vector<std::function<void(iota_t&)>>>>)
      override;

  std::shared_ptr<donk::ecs::EcsManager> EcsManager() override {
    return ecs_manager_;
  }

  // Make requires both the core type (via Derived) and the subpath (via path)
  // in order to properly construct a new object.
  template <typename Derived>
  std::shared_ptr<Derived> Make(path_t path) {
    auto iota = std::make_shared<Derived>(path);
    while (!path.IsRoot()) {
      auto prototypes = FindPrototype(path);
      for (auto register_func : prototypes) {
        register_func(*iota);
      }
      path = path.Parent();
    }

    return iota;
  }

  template <typename Derived>
  std::shared_ptr<Derived> Make(preset_t preset) {
    if (preset.entity_id() != 0) {
      throw std::runtime_error(
          "cannot return iota from a preset with existing entity_id");
    }

    auto iota = Make<Derived>(preset.path());
    iota->Apply(preset);
    ecs_manager_->Register(iota);
    return iota;
  }

  void ExitOnEmptyQueue() { exit_on_empty_queue_ = true; }

  running_proc_info& QueueChild(std::shared_ptr<iota_t> iota, std::string name,
                                proc_args_t& args) override;
  running_proc_info& QueueProc(std::shared_ptr<iota_t> iota, std::string name,
                               proc_args_t& args) override;
  running_proc_info& QueueSpawn(transpiled_proc spawn,
                                proc_args_t& args) override;

  void Run() {
    spdlog::info("Donk Interpreter active.");
    active_ = true;
    time_keeper_->Start();
  }

  void Stop() override { active_ = false; }

  bool Active() override { return active_; }

  void Tick() {
    if (time_keeper_->Advanced()) {
      auto t = time_keeper_->Tick();
      cppcoro::sync_wait(scheduler_->Work(t));
      // spdlog::info(scheduler_->DEBUG__OutputState());
    }
    if (scheduler_->empty() && exit_on_empty_queue_) {
      spdlog::info("Stopping on empty scheduler.");
      Stop();
    }
  }

  std::shared_ptr<iota_t> Global() override { return global_scope_; }

 private:
  Interpreter();

  // Returns the list of registration functions associated with a given path.
  std::vector<std::function<void(donk::iota_t&)>> FindPrototype(path_t path);

  std::shared_ptr<donk::iota_t> global_scope_;
  std::shared_ptr<
      std::map<path_t, std::vector<std::function<void(donk::iota_t&)>>>>
      prototypes_;
  std::shared_ptr<donk::api::world::world_coretype> world_;
  std::shared_ptr<donk::resources::Importer> importer_;
  std::shared_ptr<donk::mapping::MapRoster> map_roster_;
  std::shared_ptr<donk::ecs::EcsManager> ecs_manager_;
  std::shared_ptr<donk::scheduler::Scheduler> scheduler_;
  std::shared_ptr<donk::scheduler::TimeKeeper> time_keeper_;

  bool active_;
  bool exit_on_empty_queue_ = false;
};

}  // namespace internal
}  // namespace donk

#endif  // __DONK_INTERPRETER_INTERPRETER_H__
