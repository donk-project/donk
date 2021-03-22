// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/interpreter/interpreter.h"

#include <memory>
#include <string>
#include <type_traits>
#include <variant>

#include "absl/memory/memory.h"
#include "absl/strings/str_split.h"
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
#include "donk/core/procs.h"
#include "donk/core/vars.h"
#include "donk/interpreter/importer.h"
#include "donk/interpreter/timekeeper.h"
#include "donk/mapping/map_view.h"
#include "entt/entity/fwd.hpp"
#include "entt/entity/storage.hpp"
#include "entt/entity/view.hpp"
#include "fmt/format.h"
#include "spdlog/spdlog.h"

namespace donk {
namespace internal {

Interpreter::Interpreter() : donk::Interpreter() {
  ecs_manager_ = std::make_shared<donk::ecs::EcsManager>();
  time_keeper_ = std::make_shared<donk::scheduler::TimeKeeper>();
}

std::shared_ptr<iota_t> Interpreter::MakeArbitrary(std::string str) {
  path_t path(str);
  if (path.HasRootType("/datum/atom/movable/mob")) {
    return Make<donk::api::datum::atom::movable::mob::mob_coretype>(path);
  } else if (path.HasRootType("/datum/atom/movable/obj")) {
    return Make<donk::api::datum::atom::movable::obj::obj_coretype>(path);
  } else if (path.HasRootType("/datum/atom/movable")) {
    return Make<donk::api::datum::atom::movable::movable_coretype>(path);
  } else if (path.HasRootType("/client")) {
    return Make<donk::api::client::client_coretype>(path);
  } else if (path.HasRootType("/database/query")) {
    return Make<donk::api::database::query::query_coretype>(path);
  } else if (path.HasRootType("/database")) {
    return Make<donk::api::database::database_coretype>(path);
  } else if (path.HasRootType("/datum/atom/area")) {
    return Make<donk::api::datum::atom::area::area_coretype>(path);
  } else if (path.HasRootType("/datum/atom/turf")) {
    return Make<donk::api::datum::atom::turf::turf_coretype>(path);
  } else if (path.HasRootType("/datum/atom")) {
    return Make<donk::api::datum::atom::atom_coretype>(path);
  } else if (path.HasRootType("/datum")) {
    return Make<donk::api::datum::datum_coretype>(path);
  } else if (path.HasRootType("/exception")) {
    return Make<donk::api::exception::exception_coretype>(path);
  } else if (path.HasRootType("/icon")) {
    return Make<donk::api::icon::icon_coretype>(path);
  } else if (path.HasRootType("/image")) {
    return Make<donk::api::image::image_coretype>(path);
  } else if (path.HasRootType("/list")) {
    return Make<donk::api::list::list_coretype>(path);
  } else if (path.HasRootType("/matrix")) {
    return Make<donk::api::matrix::matrix_coretype>(path);
  } else if (path.HasRootType("/mutable_appearance")) {
    return Make<donk::api::mutable_appearance::mutable_appearance_coretype>(
        path);
  } else if (path.HasRootType("/regex")) {
    return Make<donk::api::regex::regex_coretype>(path);
  } else if (path.HasRootType("/savefile")) {
    return Make<donk::api::savefile::savefile_coretype>(path);
  } else if (path.HasRootType("/sound")) {
    return Make<donk::api::sound::sound_coretype>(path);
  } else if (path.HasRootType("/world")) {
    return Make<donk::api::world::world_coretype>(path);
  }
  return Make<iota_t>(path);
}

void Interpreter::CreateWorld() {
  world_ = Make<::donk::api::world::world_coretype>(path_t("/world"));
}

void Interpreter::RegisterCoreprocs() {
  global_scope_ = Make<::donk::iota_t>(path_t("/"));
  auto root = prototypes_->find(donk::path_t("/"));
  if (root != prototypes_->end()) {
    for (auto reg : root->second) {
      reg(*global_scope_);
    }
  }
}

std::shared_ptr<iota_t> Interpreter::world() { return world_; }

std::shared_ptr<var_t> Interpreter::Corevar(std::string s) {
  return global_scope_->v(s);
}

std::shared_ptr<donk::mapping::MapRoster> Interpreter::map() {
  return map_roster_;
}

std::pair<path_t, std::vector<std::function<void(donk::iota_t&)>>>
Interpreter::FindPrototype(path_t path) {
  for (auto const& result : *prototypes_) {
    if (result.first == path) {
      return result;
    }
  }

  std::vector<path_t> keys;
  for (auto const& pair : *prototypes_) {
    keys.push_back(pair.first);
  }
  throw std::runtime_error(
      fmt::format("asked for unknown iota {}, valid iotas: {}", path,
                  fmt::join(keys, ", ")));
}

void Interpreter::UpdateUuidLinks(
    std::map<donk::uuid_t, std::vector<std::string>>& update_uuid_varnames) {
  auto view = EcsManager()
                  ->registry()
                  .view<donk::uuid_t, std::shared_ptr<donk::iota_t>>();
  std::map<donk::uuid_t, std::shared_ptr<donk::iota_t>> mapping;

  for (auto entity : view) {
    auto& uuid = view.get<donk::uuid_t>(entity);
    auto iota = view.get<std::shared_ptr<donk::iota_t>>(entity);
    mapping.insert_or_assign(uuid, iota);
  }

  auto view2 = ecs_manager_->registry_
                   .view<donk::uuid_t, std::shared_ptr<donk::iota_t>>();
  for (auto entity : view2) {
    auto& uuid = view.get<donk::uuid_t>(entity);
    auto& var_table =
        view.get<std::shared_ptr<donk::iota_t>>(entity)->var_table();
    auto uuid_varnames = update_uuid_varnames.find(uuid);
    if (uuid_varnames != update_uuid_varnames.end()) {
      spdlog::info("Updating UUID {}", uuid);
      for (auto& name : uuid_varnames->second) {
        spdlog::info("UUID {} Name {}", uuid, name);
        auto old_uuid = var_table.Lookup(name)->get_uuid();
        auto redirect_to = mapping.find(old_uuid);
        if (redirect_to != mapping.end()) {
          auto var = var_table.Lookup(name);
          spdlog::info(
              "Update var_table {} uuid/{}, var={}, from uuid {} to iota={}",
              var_table, uuid, *var, old_uuid, *redirect_to->second);
          *var = redirect_to->second;
        }
      }
    }
  }
}

void Interpreter::ResetMaps() { ResetMapRoster(*map_roster_); }

void Interpreter::SetRegistrationFunctions(
    std::shared_ptr<std::map<path_t, std::vector<std::function<void(iota_t&)>>>>
        prototypes) {
  prototypes_ = prototypes;
}

void Interpreter::Import(std::string root_filename) {
  importer_ = std::make_shared<donk::resources::Importer>(root_filename);
  importer_->Ingest();
  importer_->Parse();
  map_roster_ = std::make_shared<donk::mapping::MapRoster>(importer_->maps());
}

void Interpreter::ResetMapRoster(donk::mapping::MapRoster& roster) {
  int tiles = 0;
  for (auto& map_view : *roster.map_views()) {
    for (auto& map_tile : *map_view.flat_map()) {
      for (auto& preset : *map_tile->presets()) {
        if (preset.path().IsTurf()) {
          map_tile->turf(
              Make<donk::api::datum::atom::turf::turf_coretype>(preset));
        } else if (preset.path().IsArea()) {
          map_tile->area(
              Make<donk::api::datum::atom::area::area_coretype>(preset));
        } else if (preset.path().IsObj()) {
          map_tile->AddObj(
              Make<donk::api::datum::atom::movable::obj::obj_coretype>(preset));
        } else if (preset.path().IsMob()) {
          map_tile->AddMob(
              Make<donk::api::datum::atom::movable::mob::mob_coretype>(preset));
        }
      }
      tiles++;
    }
  }
  spdlog::info("Reset {} map tiles.", tiles);
}

}  // namespace internal
}  // namespace donk
