#ifndef __DONK_PROJECT_DONK_MAPPING_MAP_ROSTER_H__
#define __DONK_PROJECT_DONK_MAPPING_MAP_ROSTER_H__

#include <map>
#include <string>

#include "donk/mapping/map_tile.h"
#include "donk/mapping/map_view.h"
#include "donk/mapping/tgm_parser.h"

namespace donk {
namespace mapping {

class MapRoster {
 public:
  MapRoster() : map_views_(std::make_shared<std::vector<MapView>>()) {}

  void AddMap(std::string name, map_t map) {
    map_views_->push_back(MapView(map));
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

#endif  // __DONK_PROJECT_DONK_MAPPING_MAP_ROSTER_H__
