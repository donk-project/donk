// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __SNOWFROST_DONK_CORE_PATH_H__
#define __SNOWFROST_DONK_CORE_PATH_H__

#include <algorithm>
#include <string>
#include <vector>

#include "fmt/format.h"
#include "spdlog/spdlog.h"

namespace donk {

class path_t {
 public:
  path_t(const std::string&);

  // Returns the fully-qualified path as a string.
  std::string fq() const;

  // Returns whether the path is a core BYOND type.
  bool HasRootType(std::string s) const;

  bool IsArea() const;
  bool IsTurf() const;
  bool IsObj() const;
  bool IsMob() const;
  bool IsCoretype() const;

  // Returns the path with its last-most piece removed.
  path_t Parent() const;
  // Returns the path with the added piece c.
  path_t Child(std::string c) const;

  // Returns whether or not the path is the root, '/'.
  bool IsRoot() const;

  bool operator<(const path_t& rhs) const { return name_ < rhs.name_; }
  bool operator==(const path_t& rhs) const { return name_ == rhs.name_; }
  bool operator!=(const path_t& rhs) const { return !operator==(rhs); }

 private:
  void FullyQualify(std::string& s);

  std::string name_;
};

}  // namespace donk

namespace fmt {

template <>
struct fmt::formatter<donk::path_t> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(donk::path_t const& p, FormatContext& ctx) {
    return fmt::format_to(ctx.out(), "path_t<{}>", p.fq());
  }
};

}  // namespace fmt

#endif  // __SNOWFROST_DONK_CORE_PATH_H__
