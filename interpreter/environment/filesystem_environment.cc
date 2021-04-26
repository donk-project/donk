// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/interpreter/environment/filesystem_environment.h"

#include <cstdlib>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "donk/image/dmi_parser.h"
#include "donk/image/dmi_reader.h"
#include "donk/image/import_icon_file.h"
#include "donk/interpreter/environment/environment.h"
#include "donk/mapping/import_map_file.h"
#include "donk/mapping/tgm_parser.h"
#include "spdlog/spdlog.h"

namespace donk {
namespace environment {

void FilesystemEnvironment::ParseAll() {
  for (const auto& [path, fullpath] : dmm_paths_) {
    auto map = donk::mapping::ImportMapFile(fullpath);
    if (map == nullptr) {
      spdlog::critical("Failed to parse map file {}.", fullpath);
    } else {
      maps_[path] = map;
    }
  }

  for (const auto& [path, fullpath] : dmi_paths_) {
    auto icon = donk::image::ImportIconFile(fullpath);
    if (icon == nullptr) {
      spdlog::critical("Failed to parse icon file {}.", fullpath);
    } else {
      icon->source_filename = path;
      icons_[path] = icon;
    }
  }
}

bool FilesystemEnvironment::HasMap(std::string name) {
  for (const auto& [localpath, _] : dmm_paths_) {
    if (localpath == name) {
      return true;
    }
  }
  return false;
}

std::shared_ptr<donk::mapping::map_t> FilesystemEnvironment::GetMapData(
    std::string name) {
  for (const auto& [localpath, map] : maps_) {
    if (localpath == name) {
      return map;
    }
  }
  return nullptr;
}

std::vector<std::string> FilesystemEnvironment::GetMapNames() {
  std::vector<std::string> result;
  for (const auto& [localpath, _] : dmm_paths_) {
    result.push_back(localpath);
  }
  return result;
}

bool FilesystemEnvironment::HasIcon(std::string name) {
  for (const auto& [localpath, _] : dmi_paths_) {
    if (localpath == name) {
      return true;
    }
  }
  return false;
}

std::shared_ptr<donk::image::dmi_data_t> FilesystemEnvironment::GetIconData(
    std::string name) {
  for (const auto& [localpath, icon] : icons_) {
    if (localpath == name) {
      return icon;
    }
  }
  return nullptr;
}

void FilesystemEnvironment::DEBUG__LogFindings() {
  spdlog::info("FilesystemEnvironment({})", root_path_);
  spdlog::info("{} Icons", dmi_paths_.size());
  for (const auto& [localpath, fullpath] : dmi_paths_) {
    spdlog::info("{} | {}", localpath, fullpath);
  }
  spdlog::info("{} Maps", dmm_paths_.size());
  for (const auto& [localpath, fullpath] : dmm_paths_) {
    spdlog::info("{} | {}", localpath, fullpath);
  }
}

int FilesystemEnvironment::GetIconCount() { return dmi_paths_.size(); }

}  // namespace environment
}  // namespace donk
