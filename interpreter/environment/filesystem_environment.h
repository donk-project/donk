#ifndef __DONK_PROJECT_DONK_INTERPRETER_ENVIRONMENT_FILESYSTEM_ENVIRONMENT_H__
#define __DONK_PROJECT_DONK_INTERPRETER_ENVIRONMENT_FILESYSTEM_ENVIRONMENT_H__

#include <cstdlib>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "donk/image/dmi_parser.h"
#include "donk/image/dmi_reader.h"
#include "donk/interpreter/environment/environment.h"
#include "donk/mapping/tgm_parser.h"

namespace donk {
namespace environment {

// FilesystemEnvironment interfaces with the filesystem in order to tell the
// interpreter or any other interested caller what files have actually been
// provided in the Dreammaker environment. It is used in certain app bundling
// conditions, namely macOS Application bundles. In this case, only the root
// path to the DM resources are required.
class FilesystemEnvironment : public Environment {
 public:
  FilesystemEnvironment(std::string root_path) : root_path_(root_path) {
    for (auto& p : std::filesystem::directory_iterator(root_path_)) {
      std::filesystem::path path(p.path().lexically_relative(root_path_));
      if (path.extension() == ".dmm") {
        dmm_paths_[path.string()] = p.path();
      } else if (path.extension() == ".dmi") {
        dmi_paths_[path.string()] = p.path();
      }
    }

    ParseAll();
  }

  bool HasMap(std::string name) override;
  std::shared_ptr<donk::mapping::map_t> GetMapData(std::string name) override;
  std::vector<std::string> GetMapNames() override;

  bool HasIcon(std::string name) override;
  std::shared_ptr<donk::image::dmi_data_t> GetIconData(
      std::string name) override;
  int GetIconCount() override;

  void DEBUG__LogFindings() override;
  std::string GetFullIconPath(std::string name) override {
    if (!HasIcon(name)) {
      return "";
    }
    return dmi_paths_[name].string();
  }

 private:
  void ParseAll();

  std::map<std::string, std::filesystem::path> dmi_paths_;
  std::map<std::string, std::filesystem::path> dmm_paths_;

  std::map<std::string, std::shared_ptr<donk::image::dmi_data_t>> icons_;
  std::map<std::string, std::shared_ptr<donk::mapping::map_t>> maps_;

  std::string root_path_;
};

}  // namespace environment
}  // namespace donk

#endif  // __DONK_PROJECT_DONK_INTERPRETER_ENVIRONMENT_FILESYSTEM_ENVIRONMENT_H__
