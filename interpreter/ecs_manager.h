// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_INTERPRETER_ECS_MANAGER_H__
#define __DONK_INTERPRETER_ECS_MANAGER_H__

#include <chrono>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include "donk/core/iota.h"
#include "donk/core/preset.h"
#include "donk/core/procs.h"
#include "donk/core/vars.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/storage.hpp"
#include "entt/entity/view.hpp"
#include "entt/entt.hpp"
#include "fmt/format.h"
#include "spdlog/spdlog.h"

namespace pollux {

namespace client {
namespace states {
class ClientRoom;
}  // namespace states
}  // namespace client

namespace networking {
class ProtoDeserializers;
class ProtoSerializers;
}  // namespace networking

}  // namespace pollux

namespace donk {
namespace internal {
class Interpreter;
}  // namespace internal

namespace ecs {

class EcsManager {
 public:
  EcsManager()
      : last_created_(1),
        can_update_entity_id_(true),
        active_entity_ids_(),
        highest_known_entity_id_(0),
        inserted_count_(0) {}

  void Update(float);

  donk::entity_id Register(std::shared_ptr<iota_t>);

  void RegisterManually(std::shared_ptr<iota_t>, donk::preset_t&);

  entt::registry& registry() { return registry_; }

  void Merge(donk::preset_t preset) {
    auto entity = GetRegistered(preset.entity_id());
    auto view = registry().view<std::shared_ptr<donk::iota_t>>();
    auto iota = view.get<std::shared_ptr<donk::iota_t>>(entity);
    iota->Apply(preset);
  }

  entt::entity GetRegistered(donk::entity_id entity_id) {
    for (auto& pair : entities_) {
      if (pair.first == entity_id) {
        return pair.second;
      }
    }
    throw std::runtime_error(
        fmt::format("unable to find entity_id {}", entity_id));
  }

  bool IsRegistered(donk::entity_id entity_id) {
    for (auto& pair : entities_) {
      if (pair.first == entity_id) {
        return true;
      }
    }
    return false;
  }

 private:
  donk::entity_id last_created_;
  entt::registry registry_;
  bool can_update_entity_id_;
  std::set<donk::entity_id> active_entity_ids_;
  std::map<donk::entity_id, entt::entity> entities_;
  int highest_known_entity_id_;
  int inserted_count_;
  friend class ::pollux::networking::ProtoSerializers;
  friend class ::pollux::networking::ProtoDeserializers;
  friend class ::pollux::client::states::ClientRoom;
  friend class donk::internal::Interpreter;
};

}  // namespace ecs
}  // namespace donk

#endif  // __DONK_INTERPRETER_ECS_MANAGER_H__
