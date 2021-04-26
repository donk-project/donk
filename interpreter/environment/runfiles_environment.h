// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_PROJECT_DONK_INTERPRETER_ENVIRONMENT_RUNFILES_ENVIRONMENT_H__
#define __DONK_PROJECT_DONK_INTERPRETER_ENVIRONMENT_RUNFILES_ENVIRONMENT_H__
#include <filesystem>

#include "donk/interpreter/environment/environment.h"
#include "tools/cpp/runfiles/runfiles.h"

namespace donk {
namespace environment {

static constexpr char RUNFILES_MANIFEST_FILE[] = "RUNFILES_MANIFEST_FILE";

// RunfilesEnvironment is used for sourcing DM data in the majority of
// situations. Runfiles data is provided to a rule via Bazel, which makes a
// manifest available to the running program in order to find the location of
// the DM environment data. Runfiles can be completely discrete and be sourced
// from different rules, but must all share a "universal prefix": the root
// directory in the target Runfiles where all data is placed.
//
// While Runfiles can more accurately map a directory via symlink on some OSes
// (macOS) this approach is not viable on others (Windows), so the manifest is
// read in all cases for simplicity's sake.
class RunfilesEnvironment : public Environment {
 public:
  RunfilesEnvironment(std::string runfiles_path, std::string universal_prefix);

  bool HasMap(std::string name) override;
  std::shared_ptr<donk::mapping::map_t> GetMapData(std::string name) override;
  std::vector<std::string> GetMapNames() override;

  bool HasIcon(std::string name) override;
  std::shared_ptr<donk::image::dmi_data_t> GetIconData(
      std::string name) override;
  int GetIconCount() override;
  std::string GetFullIconPath(std::string name) override {
    if (!HasIcon(name)) {
      return "";
    }
    return dmi_paths_[name].string();
  }

  void DEBUG__LogFindings() override;

 private:
  void ParseAll();
  bool ParseManifest(const std::string& path, std::string* error);

  std::map<std::string, std::filesystem::path> dmi_paths_;
  std::map<std::string, std::filesystem::path> dmm_paths_;

  std::map<std::string, std::shared_ptr<donk::image::dmi_data_t>> icons_;
  std::map<std::string, std::shared_ptr<donk::mapping::map_t>> maps_;

  bazel::tools::cpp::runfiles::Runfiles* runfiles_;
  std::string runfiles_path_;
  std::string universal_prefix_;
};

}  // namespace environment
}  // namespace donk

#endif  // __DONK_PROJECT_DONK_INTERPRETER_ENVIRONMENT_RUNFILES_ENVIRONMENT_H__
