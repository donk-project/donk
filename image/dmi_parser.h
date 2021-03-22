// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __SNOWFROST_DONK_IMAGE_DMI_PARSER_H__
#define __SNOWFROST_DONK_IMAGE_DMI_PARSER_H__

#include <string>
#include <vector>

#include "peglib.h"

namespace donk {
namespace image {

struct dmi_state_t {
  std::string name;
  int dirs;
  int frames;
  int rewind;
  std::vector<float> delay;
};

struct dmi_data_t {
  std::string source_filename;
  int icon_width, icon_height, image_width, image_height;
  std::vector<dmi_state_t> states;
};

peg::parser Parser();

}  // namespace image
}  // namespace donk

#endif  // __SNOWFROST_DONK_IMAGE_DMI_PARSER_H__
