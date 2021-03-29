// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/interpreter/ecs_manager.h"

#include <string>

#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"
#include "donk/core/vars.h"

namespace donk {
namespace ecs {

void EcsManager::Update(float delta) {}

donk::entity_id EcsManager::Register(std::shared_ptr<iota_t> i) {
  if (!can_update_entity_id_) {
    throw std::runtime_error(
        fmt::format("no new entity_ids can be chosen registering iota {}", i));
  }
  auto entity = registry_.create();
  donk::entity_id entity_id = ++last_created_;
  i->entity_id_ = entity_id;
  registry_.emplace<donk::path_t>(entity, i->GetPath());
  registry_.emplace<donk::entity_id>(entity, entity_id);
  registry_.emplace<std::shared_ptr<donk::iota_t>>(entity, i);

  active_entity_ids_.insert(entity_id);
  return entity_id;
}

void EcsManager::RegisterManually(std::shared_ptr<iota_t> iota,
                                  donk::preset_t& preset) {
  auto entity = registry_.create();
  iota->entity_id_ = preset.entity_id();
  iota->var_table_.RegisterPreset(preset);

  registry_.emplace<donk::path_t>(entity, preset.path());
  registry_.emplace<donk::entity_id>(entity, preset.entity_id());
  registry_.emplace<std::shared_ptr<donk::iota_t>>(entity, iota);

  active_entity_ids_.insert(preset.entity_id());
  can_update_entity_id_ = false;
}

}  // namespace ecs
}  // namespace donk