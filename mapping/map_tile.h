#ifndef __DONK_PROJECT_DONK_MAPPING_MAP_TILE_H__
#define __DONK_PROJECT_DONK_MAPPING_MAP_TILE_H__

#include <vector>

#include "donk/core/iota.h"
#include "donk/core/prefab.h"
#include "donk/core/preset.h"

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

}  // namespace mapping
}  // namespace donk

#endif  // __DONK_PROJECT_DONK_MAPPING_MAP_TILE_H__
