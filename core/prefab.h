// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_CORE_PREFAB_H__
#define __DONK_CORE_PREFAB_H__

#include "donk/core/path.h"

namespace donk {

class prefab_t {
 public:
  prefab_t(std::string path) : path_(path) {}

  const path_t path() const { return path_; }

 private:
  donk::path_t path_;
};

}  // namespace donk

#endif  // __DONK_CORE_PREFAB_H__
