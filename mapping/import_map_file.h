#ifndef __DONK_PROJECT_DONK_MAPPING_IMPORT_MAP_FILE_H__
#define __DONK_PROJECT_DONK_MAPPING_IMPORT_MAP_FILE_H__

#include <cstdlib>
#include <string>

#include "donk/mapping/tgm_parser.h"
#include "spdlog/spdlog.h"

namespace donk {
namespace mapping {

std::shared_ptr<donk::mapping::map_t> ImportMapFile(std::string fullpath) {
  std::string line;
  std::stringstream result;
  std::ifstream dmm_file(fullpath, std::ios_base::in | std::ios_base::binary);
  if (dmm_file.is_open()) {
    while (dmm_file.good()) {
      getline(dmm_file, line);
      result << line;
    }
    dmm_file.close();
    auto parser = donk::mapping::Parser();
    auto map = std::make_shared<donk::mapping::map_t>();
    bool rv = parser.parse(result.str().c_str(), *map);
    if (!rv) {
      spdlog::error("Unable to parse map {}", fullpath);
    }
    return map;
  } else {
    throw std::runtime_error(fmt::format("unable to open file {}", fullpath));
  }
}

}  // namespace mapping
}  // namespace donk

#endif  // __DONK_PROJECT_DONK_MAPPING_IMPORT_MAP_FILE_H__
