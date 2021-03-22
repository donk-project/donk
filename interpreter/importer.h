// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_INTERPRETER_IMPORTER_H__
#define __DONK_INTERPRETER_IMPORTER_H__

#include <map>
#include <string>
#include <vector>

#include "donk/image/dmi_parser.h"
#include "donk/image/dmi_reader.h"
#include "donk/mapping/tgm_parser.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace donk {
namespace resources {

// TODO: This value relies on the name of the DME, so can't be hardcoded
inline static const std::string UNIVERSAL_PREFIX =
    "snowfrost/snowfrost/FalacyTut/";

// Importer handles management for maps, icons, sounds, and other resources
// retrieved from the filesystem.
class Importer {
 public:
  Importer(std::string runfile_path) : runfile_path_(runfile_path) {}

  // Finds all relevant resource files on the filesystem and stores their
  // metadata.
  void Ingest();
  // Parses data from discovered resource files.
  void Parse();

  std::string get_root() { return root_; }
  std::map<std::string, donk::mapping::map_t>& maps() { return maps_; }
  std::map<std::string, donk::image::dmi_data_t>& icon_data() {
    return icon_data_;
  }

 private:
  std::string root_;
  std::string runfile_path_;
  std::vector<std::filesystem::path> dmi_paths_;
  std::vector<std::filesystem::path> dmm_paths_;
  std::map<std::string, donk::image::dmi_data_t> icon_data_;
  std::map<std::string, donk::mapping::map_t> maps_;
  bazel::tools::cpp::runfiles::Runfiles* runfiles_;
};

}  // namespace resources
}  // namespace donk

#endif  // __DONK_INTERPRETER_IMPORTER_H__
