// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_CORE_RESOURCE_H__
#define __DONK_CORE_RESOURCE_H__

#include <string>

namespace donk {

class resource_t {
 public:
  resource_t(std::string name) : name_(name) {}

  const std::string name() const { return name_; }

 private:
  std::string name_;
};

}  // namespace donk

#endif  // __DONK_CORE_RESOURCE_H__
