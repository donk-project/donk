// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/interpreter/importer.h"

#include <cstdlib>
#include <filesystem>

#include "spdlog/spdlog.h"

namespace donk {
namespace resources {

static constexpr char RUNFILES_MANIFEST_FILE[] = "RUNFILES_MANIFEST_FILE";
static constexpr char RUNFILES_DIR[] = "RUNFILES_DIR";

void Importer::Ingest() {
  std::string error;
  runfiles_ =
      bazel::tools::cpp::runfiles::Runfiles::Create(runfile_path_, &error);
  if (runfiles_ == nullptr) {
    throw std::runtime_error(fmt::format("error loading runfiles: {}", error));
  }

  auto env_vars = runfiles_->EnvVars();
  for (auto& pair : env_vars) {
    if (pair.first == "RUNFILES_MANIFEST_FILE") {
      if (pair.second != "") {
        auto result = ParseManifest(pair.second, &error);
        if (!result) {
          throw std::runtime_error(
              fmt::format("error reading manifest: {}", error));
        }
      }
    }
  }
}

void Importer::Parse() {
  for (const auto& path : dmm_paths_) {
    std::stringstream result;
    std::string line;
    auto fullpath = UNIVERSAL_PREFIX + path.string();
    auto resolvedpath = runfiles_->Rlocation(fullpath);
    std::ifstream dmm_file(resolvedpath,
                           std::ios_base::in | std::ios_base::binary);
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
          fmt::format("unable to open file {}", resolvedpath));
    }
  }

  for (const auto& path : dmi_paths_) {
    auto fullpath = UNIVERSAL_PREFIX + path.string();
    auto resolvedpath = runfiles_->Rlocation(fullpath);
    donk::image::DMIReader reader(resolvedpath);
    reader.Read();
    if (reader.dmi_data() == "") {
      throw std::runtime_error(
          fmt::format("no DMI data found in image {}", resolvedpath));
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

// ParseManifest is taken from Bazel's tools/cpp/runfiles/runfiles_src.cc
// licensed under Apache 2.0.
//
// Manifests exist on Windows where symlinks are dodgy so while Runfiles works
// on both platforms if you know explicit file names, it cannot be used to find
// some root runfile directory and iterate over child paths, since its sole
// representation is a key-value pair of logical filename to physical filename.
//
// So on Windows we parse the manifest ourselves to get the file paths we want
// out of it.
bool Importer::ParseManifest(const std::string& path, std::string* error) {
  std::ifstream stm(path);
  if (!stm.is_open()) {
    if (error) {
      std::ostringstream err;
      err << "ERROR: " << __FILE__ << "(" << __LINE__
          << "): cannot open runfiles manifest \"" << path << "\"";
      *error = err.str();
    }
    return false;
  }
  std::string line;
  std::getline(stm, line);
  size_t line_count = 1;
  while (!line.empty()) {
    std::string::size_type idx = line.find_first_of(' ');
    if (idx == std::string::npos) {
      if (error) {
        std::ostringstream err;
        err << "ERROR: " << __FILE__ << "(" << __LINE__
            << "): bad runfiles manifest entry in \"" << path << "\" line #"
            << line_count << ": \"" << line << "\"";
        *error = err.str();
      }
      return false;
    }
    auto logical_filename = std::filesystem::path(line.substr(0, idx))
                                .lexically_relative(UNIVERSAL_PREFIX);
    if (logical_filename.extension() == ".dmm") {
      dmm_paths_.push_back(logical_filename);
    } else if (logical_filename.extension() == ".dmi") {
      dmi_paths_.push_back(logical_filename);
    }
    std::getline(stm, line);
    ++line_count;
  }
  return true;
}

}  // namespace resources
}  // namespace donk
