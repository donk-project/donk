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
#include "donk/mapping/map_tile.h"
#include "donk/mapping/tgm_parser.h"
#include "spdlog/spdlog.h"

namespace donk {
namespace mapping {

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

}  // namespace mapping
}  // namespace donk

#endif  // __SNOWFROST_DONK_MAPPING_MAP_VIEW_H__
