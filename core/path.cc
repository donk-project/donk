// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/core/path.h"

#include <sstream>

namespace donk {

path_t::path_t(const std::string& name) : name_(name) {
  if (name_.empty()) {
    throw std::runtime_error("path_t input must never be empty");
  }
  if (name_.at(0) != '/') {
    throw std::runtime_error(fmt::format(
        "path_t input must always start with \"/\" (but was {})", name_));
  }
  FullyQualify(name_);
}

std::string path_t::fq() const { return name_; }

bool path_t::HasRootType(std::string s) const { return name_.rfind(s, 0) == 0; }

bool path_t::IsArea() const { return HasRootType("/datum/atom/area"); }
bool path_t::IsTurf() const { return HasRootType("/datum/atom/turf"); }
bool path_t::IsObj() const { return HasRootType("/datum/atom/movable/obj"); }
bool path_t::IsMob() const { return HasRootType("/datum/atom/movable/mob"); }

bool path_t::IsCoretype() const {
  if (name_ == "/") return true;
  if (name_ == "/client") return true;
  if (name_ == "/database") return true;
  if (name_ == "/database/query") return true;
  if (name_ == "/datum") return true;
  if (name_ == "/datum/atom") return true;
  if (name_ == "/datum/atom/area") return true;
  if (name_ == "/datum/atom/movable") return true;
  if (name_ == "/datum/atom/movable/mob") return true;
  if (name_ == "/datum/atom/movable/obj") return true;
  if (name_ == "/datum/atom/turf") return true;
  if (name_ == "/exception") return true;
  if (name_ == "/icon") return true;
  if (name_ == "/image") return true;
  if (name_ == "/list") return true;
  if (name_ == "/matrix") return true;
  if (name_ == "/mutable_appearance") return true;
  if (name_ == "/regex") return true;
  if (name_ == "/savefile") return true;
  if (name_ == "/sound") return true;
  if (name_ == "/world") return true;

  return false;
}

path_t path_t::Parent() const {
  if (name_ == "/area") {
    return path_t("/atom");
  }
  if (name_ == "/atom") {
    return path_t("/datum");
  }
  if (name_ == "/mob") {
    return path_t("/atom/movable");
  }
  if (name_ == "/turf") {
    return path_t("/atom");
  }
  if (name_ == "/obj") {
    return path_t("/atom/movable");
  }

  std::string delimiter = "/";
  std::vector<std::string> parts;
  std::string s(name_);
  // Shamelessly stolen from https://stackoverflow.com/questions/14265581
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    parts.push_back(token);
    s.erase(0, pos + delimiter.length());
  }

  std::ostringstream out;
  if (parts.empty()) {
    return path_t("/");
  } else {
    std::copy(parts.begin(), parts.end() - 1,
              std::ostream_iterator<std::string>(out, "/"));
    out << parts.back();
    if (out.str() == "") {
      return path_t("/");
    }
    return path_t(out.str());
  }
}

path_t path_t::Child(std::string s) const {
  s.erase(0, s.find_first_not_of("/"));
  s.erase(s.find_last_not_of("/") + 1);
  return path_t(fmt::format("{}/{}", name_, s));
}

bool path_t::IsRoot() const { return name_ == "/"; }

void path_t::FullyQualify(std::string& s) {
  if (s.rfind("/area", 0) == 0) {
    s.insert(0, "/datum/atom");
  } else if (s.rfind("/atom", 0) == 0) {
    s.insert(0, "/datum");
  } else if (s.rfind("/mob", 0) == 0) {
    s.insert(0, "/datum/atom/movable");
  } else if (s.rfind("/turf", 0) == 0) {
    s.insert(0, "/datum/atom");
  } else if (s.rfind("/obj", 0) == 0) {
    s.insert(0, "/datum/atom/movable");
  }
}

}  // namespace donk
