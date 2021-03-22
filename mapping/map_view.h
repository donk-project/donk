// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __SNOWFROST_DONK_MAPPING_MAP_VIEW_H__
#define __SNOWFROST_DONK_MAPPING_MAP_VIEW_H__

#include <map>
#include <memory>
#include <vector>

#include "donk/core/interpreter.h"
#include "donk/core/iota.h"
#include "donk/core/procs.h"
#include "donk/core/vars.h"
#include "donk/mapping/tgm_parser.h"
#include "spdlog/spdlog.h"

namespace donk {
namespace mapping {

class MapTile {
 public:
  MapTile(std::shared_ptr<std::vector<preset_t>> presets) : presets_(presets) {}

  std::shared_ptr<donk::iota_t> turf() { return turf_; }

  std::shared_ptr<std::vector<preset_t>> presets() { return presets_; }

  void area(std::shared_ptr<donk::iota_t> area) { area_ = area; }

  void turf(std::shared_ptr<donk::iota_t> turf) { turf_ = turf; }

  void AddMob(std::shared_ptr<donk::iota_t> mob) { mobs_.push_back(mob); }

  void AddObj(std::shared_ptr<donk::iota_t> obj) { objs_.push_back(obj); }

 private:
  std::shared_ptr<std::vector<preset_t>> presets_;
  std::vector<std::shared_ptr<donk::iota_t>> objs_;
  std::shared_ptr<donk::iota_t> area_;
  std::shared_ptr<donk::iota_t> turf_;
  std::vector<std::shared_ptr<donk::iota_t>> mobs_;
  friend class MapRoster;
};

class MapView {
 public:
  MapView(donk::mapping::map_t dmm_data)
      : dmm_data_(dmm_data),
        presets_(std::make_shared<std::map<std::string, std::vector<preset_t>>>(
            dmm_data.presets)),
        flat_map_(std::make_shared<std::vector<std::shared_ptr<MapTile>>>()) {
    map_width_ = 0;
    for (auto& [triplet, row_decl] : dmm_data.rows) {
      map_width_ = row_decl.key_names.size();
      for (int i = 0; i < row_decl.key_names.size(); i++) {
        auto key = row_decl.key_names[i];
        auto presets = std::make_shared<std::vector<donk::preset_t>>();
        for (int j = 0; j < (*presets_)[key].size(); j++) {
          auto preset = (*presets_)[key][j];
          auto p = presets->insert(presets->end(), preset);
          p->vars().insert_by_name("x", std::make_shared<var_t>(triplet.x));
          p->vars().insert_by_name("y", std::make_shared<var_t>(triplet.y + i));
          p->vars().insert_by_name("z", std::make_shared<var_t>(triplet.z));
        }
        flat_map_->push_back(std::make_shared<MapTile>(presets));
      }
    }
  }

  std::shared_ptr<MapTile> index(int x, int y) {
    try {
      return flat_map_->at(map_width_ * x + y);
    } catch (const std::out_of_range& oor) {
      spdlog::critical("MapTile::index({}, {}) out of range, flat map size {}",
                       x, y, flat_map_->size());
    }
    throw std::runtime_error("failed catastrophically to retrieve index");
  }

  std::shared_ptr<std::vector<std::shared_ptr<MapTile>>> flat_map() {
    return flat_map_;
  }

  std::shared_ptr<std::map<std::string, std::vector<preset_t>>> presets() {
    return presets_;
  }

 private:
  int map_width_;
  donk::mapping::map_t dmm_data_;
  std::shared_ptr<std::map<std::string, std::vector<preset_t>>> presets_;
  std::shared_ptr<std::vector<std::shared_ptr<MapTile>>> flat_map_;

  friend class MapRoster;
};

class MapRoster {
 public:
  MapRoster(std::map<std::string, donk::mapping::map_t> dmm_data)
      : map_views_(std::make_shared<std::vector<MapView>>()) {
    for (auto& [filename, dmm_datum] : dmm_data) {
      map_views_->push_back(MapView(dmm_datum));
    }
  }

  std::shared_ptr<MapTile> index(int x, int y, int z) {
    auto level = map_views_->at(z - 1);
    auto index = level.index(x - 1, y - 1);

    if (index == nullptr) {
      spdlog::critical("map view returning null index at {} {} {}", x, y, z);
    }

    return index;
  }

  std::shared_ptr<std::vector<MapView>> map_views() { return map_views_; }

 private:
  std::shared_ptr<std::vector<MapView>> map_views_;
};

}  // namespace mapping
}  // namespace donk

#endif  // __SNOWFROST_DONK_MAPPING_MAP_VIEW_H__
