// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __SNOWFROST_DONK_MAPPING_TGM_PARSER_H__
#define __SNOWFROST_DONK_MAPPING_TGM_PARSER_H__

#include <vector>

#include "absl/strings/str_join.h"
#include "donk/core/iota.h"
#include "donk/core/preset.h"
#include "donk/core/procs.h"
#include "donk/core/vars.h"
#include "peglib.h"
#include "spdlog/spdlog.h"

namespace donk {
namespace mapping {

struct attr_pair {
  std::string name;
  donk::var_t var;
};

struct triplet_t {
  triplet_t(int x_, int y_, int z_) : x(x_), y(y_), z(z_){};
  int x;
  int y;
  int z;
  bool operator<(const triplet_t& rhs) const {
    return x < rhs.x || y < rhs.y || z < rhs.z;
  }
};

struct row_decl_t {
  row_decl_t(triplet_t triplet_, std::vector<std::string> key_names_)
      : triplet(triplet_), key_names(key_names_){};
  row_decl_t(const row_decl_t& other)
      : triplet(other.triplet), key_names(other.key_names){};
  triplet_t triplet;
  std::vector<std::string> key_names;
};

struct key_decl_t {
  key_decl_t(std::string name_) : name(name_){};
  std::string name;
  std::vector<donk::preset_t> presets;
};

struct map_t {
  std::map<std::string, std::vector<donk::preset_t>> presets;
  std::map<triplet_t, row_decl_t> rows;
};

peg::parser Parser();

}  // namespace mapping
}  // namespace donk

#endif  // __SNOWFROST_DONK_MAPPING_TGM_PARSER_H__
