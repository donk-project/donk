#include "donk/interpreter/environment/runfiles_environment.h"

#include <sstream>
#include <string>

#include "donk/image/import_icon_file.h"
#include "donk/mapping/import_map_file.h"
#include "fmt/format.h"
#include "spdlog/spdlog.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace donk {
namespace environment {

RunfilesEnvironment::RunfilesEnvironment(std::string runfiles_path,
                                         std::string universal_prefix)
    : runfiles_path_(runfiles_path), universal_prefix_(universal_prefix) {
  std::string error;
  runfiles_ =
      bazel::tools::cpp::runfiles::Runfiles::Create(runfiles_path, &error);
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

  ParseAll();
}

void RunfilesEnvironment::ParseAll() {
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

// ParseManifest is largely sourced from Bazel's
// tools/cpp/runfiles/runfiles_src.cc licensed under Apache 2.0.
//
// Manifests exist on Windows where symlinks are dodgy so while Runfiles works
// on both platforms if you know explicit file names, it cannot be used to find
// some root runfile directory and iterate over child paths, since its sole
// representation is a key-value pair of logical filename to physical filename.
//
// So on Windows we parse the manifest ourselves to get the file paths we want
// out of it.
bool RunfilesEnvironment::ParseManifest(const std::string& path,
                                        std::string* error) {
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
                                .lexically_relative(universal_prefix_);
    auto absolute_filename = std::filesystem::path(line.substr(idx + 1));
    if (absolute_filename.extension() == ".dmm") {
      dmm_paths_[logical_filename.string()] = absolute_filename;
    } else if (absolute_filename.extension() == ".dmi") {
      dmi_paths_[logical_filename.string()] = absolute_filename;
    }
    std::getline(stm, line);
    ++line_count;
  }
  return true;
}

bool RunfilesEnvironment::HasMap(std::string name) {
  for (const auto& [localpath, _] : dmm_paths_) {
    if (localpath == name) {
      return true;
    }
  }
  return false;
}

std::shared_ptr<donk::mapping::map_t> RunfilesEnvironment::GetMapData(
    std::string name) {
  for (const auto& [localpath, map] : maps_) {
    if (localpath == name) {
      return map;
    }
  }
  return nullptr;
}

std::vector<std::string> RunfilesEnvironment::GetMapNames() {
  std::vector<std::string> result;
  for (const auto& [localpath, _] : dmm_paths_) {
    result.push_back(localpath);
  }
  return result;
}

bool RunfilesEnvironment::HasIcon(std::string name) {
  for (const auto& [localpath, _] : dmi_paths_) {
    if (localpath == name) {
      return true;
    }
  }
  return false;
}

std::shared_ptr<donk::image::dmi_data_t> RunfilesEnvironment::GetIconData(
    std::string name) {
  for (const auto& [localpath, icon] : icons_) {
    if (localpath == name) {
      return icon;
    }
  }
  return nullptr;
}

void RunfilesEnvironment::DEBUG__LogFindings() {
  spdlog::info("RunfilesEnvironment({}, {})", runfiles_path_,
               universal_prefix_);
  spdlog::info("{} Icons", dmi_paths_.size());
  for (const auto& [localpath, fullpath] : dmi_paths_) {
    spdlog::info("{} | {}", localpath, fullpath);
  }
  spdlog::info("{} Maps", dmm_paths_.size());
  for (const auto& [localpath, fullpath] : dmm_paths_) {
    spdlog::info("{} | {}", localpath, fullpath);
  }
}

int RunfilesEnvironment::GetIconCount() { return dmi_paths_.size(); }

}  // namespace environment
}  // namespace donk
