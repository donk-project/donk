// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/core/path.h"

#include "boost/ut.hpp"
#include "fmt/format.h"

int main() {
  using namespace boost::ut;

  "creation"_test = [] {
    expect(throws([] { donk::path_t empty(""); }));
    expect(throws([] { donk::path_t not_absolute("foo"); }));
  };

  "parents"_test = [] {
    donk::path_t p("/mob");
    expect(p.fq() == "/datum/atom/movable/mob");

    std::vector<std::string> parents = {"/datum/atom/movable", "/datum/atom",
                                        "/datum", "/"};
    for (int i = 0; i < parents.size(); i++) {
      p = p.Parent();
      expect(p.fq() == parents[i])
          << fmt::format("expected parent {} but was {}", parents[i], p.fq());
    }
  };

  "children"_test = [] {
    donk::path_t p("/mob");
    expect(p.Child("foo").fq() == "/datum/atom/movable/mob/foo")
        << fmt::format("unexpected child {}", p.Child("foo").fq());
    expect(p.Child("/foo/").fq() == "/datum/atom/movable/mob/foo")
        << fmt::format("unexpected child {}", p.Child("/foo/").fq());
  };
}