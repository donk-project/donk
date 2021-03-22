// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/interpreter/importer.h"

namespace donk {
namespace resources {

void Importer::Ingest() {
  std::string error;
  runfiles_ =
      bazel::tools::cpp::runfiles::Runfiles::Create(runfile_path_, &error);
  if (runfiles_ == nullptr) {
    throw std::runtime_error(fmt::format("error loading runfiles: {}", error));
  }
  root_ = runfiles_->Rlocation(UNIVERSAL_PREFIX);
  for (auto& p : std::filesystem::directory_iterator(root_)) {
    if (p.path().extension() == ".dmm") {
      dmm_paths_.push_back(p.path());
    } else if (p.path().extension() == ".dmi") {
      dmi_paths_.push_back(p.path());
    }
  }
}

void Importer::Parse() {
  for (const auto& path : dmm_paths_) {
    std::stringstream result;
    std::string line;
    std::ifstream dmm_file(path.string());
    if (dmm_file.is_open()) {
      while (dmm_file.good()) {
        getline(dmm_file, line);
        result << line;
      }
      dmm_file.close();
      auto parser = donk::mapping::Parser();
      donk::mapping::map_t map;
      bool rv = parser.parse(result.str().c_str(), map);
      if (!rv) {
        spdlog::error("Unable to parse map {}", path.string());
      } else {
        spdlog::info("{}", path.string());
      }
      maps_[path.string()] = map;
    } else {
      throw std::runtime_error(
          fmt::format("unable to open file {}", path.string()));
    }
  }

  for (const auto& path : dmi_paths_) {
    donk::image::DMIReader reader(path.string());
    reader.Read();
    if (reader.dmi_data() == "") {
      throw std::runtime_error(
          fmt::format("no DMI data found in image {}", path.string()));
    }
    donk::image::dmi_data_t dmi_data;
    auto parser = donk::image::Parser();
    parser.parse(reader.dmi_data().c_str(), dmi_data);
    dmi_data.image_width = reader.get_image_width();
    dmi_data.image_height = reader.get_image_height();
    dmi_data.source_filename = path.string();
    icon_data_.try_emplace(path.string(), dmi_data);
    spdlog::info("{}", dmi_data.source_filename);
  }
  spdlog::info("{} icons loaded.", icon_data_.size());
}

}  // namespace resources
}  // namespace donk
