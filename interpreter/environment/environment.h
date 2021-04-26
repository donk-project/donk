#ifndef __DONK_PROJECT_DONK_INTERPRETER_ENVIRONMENT_ENVIRONMENT_H__
#define __DONK_PROJECT_DONK_INTERPRETER_ENVIRONMENT_ENVIRONMENT_H__

#include <string>
#include <vector>

#include "donk/image/dmi_parser.h"
#include "donk/mapping/tgm_parser.h"

namespace donk {

class Environment {
 public:
  virtual bool HasMap(std::string name) = 0;
  virtual std::shared_ptr<donk::mapping::map_t> GetMapData(
      std::string name) = 0;
  virtual std::vector<std::string> GetMapNames() = 0;

  virtual bool HasIcon(std::string name) = 0;
  virtual std::shared_ptr<donk::image::dmi_data_t> GetIconData(
      std::string name) = 0;
  virtual int GetIconCount() = 0;
  virtual std::string GetFullIconPath(std::string name) = 0;

  virtual void DEBUG__LogFindings() = 0;
};

}  // namespace donk

#endif  // __DONK_PROJECT_DONK_INTERPRETER_ENVIRONMENT_ENVIRONMENT_H__
