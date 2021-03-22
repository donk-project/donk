// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __SNOWFROST_DONK_CORE_ICON_H__
#define __SNOWFROST_DONK_CORE_ICON_H__

#include <string>

namespace donk {

class icon_t {
 public:
  icon_t(std::string name) { name_ = name; }

 private:
  std::string name_;
};

}  // namespace donk

#endif  // __SNOWFROST_DONK_CORE_ICON_H__
