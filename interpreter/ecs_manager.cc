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

donk::uuid_t EcsManager::Register(std::shared_ptr<iota_t> i) {
  if (!can_update_uuid_) {
    throw std::runtime_error(
        fmt::format("no new uuids can be chosen registering iota {}", i));
  }
  auto entity = registry_.create();
  donk::uuid_t uuid = ++last_created_;
  i->uuid_ = uuid;
  registry_.emplace<donk::path_t>(entity, i->GetPath());
  registry_.emplace<donk::uuid_t>(entity, uuid);
  registry_.emplace<std::shared_ptr<donk::iota_t>>(entity, i);

  active_uuids_.insert(uuid);
  return uuid;
}

void EcsManager::RegisterManually(std::shared_ptr<iota_t> iota,
                                  donk::preset_t& preset) {
  auto entity = registry_.create();
  iota->uuid_ = preset.uuid();
  iota->var_table_.RegisterPreset(preset);

  registry_.emplace<donk::path_t>(entity, preset.path());
  registry_.emplace<donk::uuid_t>(entity, preset.uuid());
  registry_.emplace<std::shared_ptr<donk::iota_t>>(entity, iota);

  active_uuids_.insert(preset.uuid());
  can_update_uuid_ = false;
}

}  // namespace ecs
}  // namespace donk