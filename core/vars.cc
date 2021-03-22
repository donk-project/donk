// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/core/vars.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <type_traits>

#include "donk/core/iota.h"
#include "donk/core/preset.h"
#include "fmt/format.h"
#include "spdlog/spdlog.h"

namespace donk {

var_t::var_t() : data_(0) {}
var_t::var_t(const var_t& v) : data_(v.data_) {}
var_t::var_t(float f) : data_(f) {}
var_t::var_t(int i) : data_(i) {}
var_t::var_t(path_t p) : data_(p) {}
var_t::var_t(prefab_t p) : data_(p) {}
var_t::var_t(std::shared_ptr<iota_t> i) : data_(i) {}
var_t::var_t(std::shared_ptr<var_t> o) : data_(o->data_) {}
var_t::var_t(resource_t r) : data_(std::make_shared<resource_t>(r)) {}
var_t::var_t(std::string s) : data_(s) {}
var_t::var_t(assoc_list_t t) : data_(std::make_shared<assoc_list_t>(t)) {}

std::shared_ptr<var_t> var_t::operator-(std::shared_ptr<var_t> v) {
  if (this->IsKind<int>() && v->IsKind<int>()) {
    return std::make_shared<var_t>(this->get_int() - v->get_int());
  }
  throw std::runtime_error("unsupported types for operator-");
}

std::shared_ptr<var_t> var_t::operator*(std::shared_ptr<var_t> v) {
  if (this->IsKind<int>() && v->IsKind<int>()) {
    return std::make_shared<var_t>(this->get_int() * v->get_int());
  }
  throw std::runtime_error("unsupported types for operator*");
}

// TODO: Clean up these unholy abominations
std::shared_ptr<resource_t> var_t::get_resource() {
  try {
    return std::get<std::shared_ptr<resource_t>>(data_);
  } catch (const std::bad_variant_access& e) {
    throw std::runtime_error(fmt::format(
        "resource requested in var without one: {}, {}", *this, e.what()));
  }
}

std::shared_ptr<iota_t> var_t::get_iota() {
  try {
    return std::get<std::shared_ptr<iota_t>>(data_);
  } catch (const std::bad_variant_access& e) {
    throw std::runtime_error(fmt::format(
        "iota requested in var without one: {}, {}", *this, e.what()));
  }
}

float var_t::get_float() {
  try {
    return std::get<float>(data_);
  } catch (const std::bad_variant_access& e) {
    throw std::runtime_error(fmt::format(
        "float requested in var without one: {}, {}", *this, e.what()));
  }
}

std::string var_t::get_string() {
  try {
    return std::get<std::string>(data_);
  } catch (const std::bad_variant_access& e) {
    throw std::runtime_error(fmt::format(
        "string requested in var without one: {}, {}", *this, e.what()));
  }
}

donk::uuid_t var_t::get_uuid() {
  try {
    return std::get<donk::uuid_t>(data_);
  } catch (const std::bad_variant_access& e) {
    throw std::runtime_error(fmt::format(
        "uuid requested in var without one: {}, {}", *this, e.what()));
  }
}

int var_t::get_int() {
  try {
    return std::get<int>(data_);
  } catch (const std::bad_variant_access& e) {
    throw std::runtime_error(fmt::format(
        "int requested in var without one: {}, {}", *this, e.what()));
  }
}

donk::prefab_t var_t::get_prefab() {
  try {
    return std::get<donk::prefab_t>(data_);
  } catch (const std::bad_variant_access& e) {
    throw std::runtime_error(fmt::format(
        "prefab requested in var without one: {}, {}", *this, e.what()));
  }
}

donk::path_t var_t::get_path() {
  try {
    return std::get<donk::path_t>(data_);
  } catch (const std::bad_variant_access& e) {
    throw std::runtime_error(fmt::format(
        "path requested in var without one: {}, {}", *this, e.what()));
  }
}

std::shared_ptr<donk::assoc_list_t> var_t::get_list() {
  try {
    return std::get<std::shared_ptr<donk::assoc_list_t>>(data_);
  } catch (const std::bad_variant_access& e) {
    throw std::runtime_error(fmt::format(
        "list requested in var without one: {}, {}", *this, e.what()));
  }
}

std::shared_ptr<var_t> var_t::v(const std::string& s) {
  return get_iota()->v(s);
}

var_t& var_t::operator=(const std::shared_ptr<iota_t> i) {
  data_.emplace<std::shared_ptr<iota_t>>(i);
  return *this;
}

var_t& var_t::operator=(const std::shared_ptr<resource_t> r) {
  data_.emplace<std::shared_ptr<resource_t>>(r);
  return *this;
}

void var_table_t::Register(std::string s, var_t& var) {
  auto search = vars_->find(s);
  if (search != vars_->end()) {
    search->second->data_ = var.data_;
    return;
  }
  (*vars_)[s] = std::make_shared<var_t>(var);
}

void var_table_t::Register(std::string s) {
  auto search = vars_->find(s);
  if (search == vars_->end()) {
    vars_->insert({s, std::make_shared<donk::var_t>()});
  }
}

void var_table_t::RegisterPreset(preset_t& preset) {
  for (auto const& v : preset.vars_.vars_by_name_) {
    Register(v.first, *v.second);
  }
}

std::shared_ptr<var_t> var_table_t::Lookup(std::string s) {
  auto search = vars_->find(s);
  if (search != vars_->end()) {
    return search->second;
  } else {
    spdlog::info("lookup requested for undefined var {}", s);
    Register(s);
    return Lookup(s);
  }
}

var_t& var_t::operator+=(std::string s) {
  if (!IsKind<std::shared_ptr<donk::assoc_list_t>>()) {
    data_ = std::make_shared<donk::assoc_list_t>();
  }

  std::get<std::shared_ptr<donk::assoc_list_t>>(data_)->operator+=(
      std::make_shared<donk::var_t>(s));

  return *this;
}

var_t& var_t::operator+=(std::shared_ptr<var_t> v) {
  if (!IsKind<std::shared_ptr<donk::assoc_list_t>>()) {
    data_ = std::make_shared<donk::assoc_list_t>();
  }

  std::get<std::shared_ptr<donk::assoc_list_t>>(data_)->operator+=(v);

  return *this;
}

var_t& var_t::operator+=(std::shared_ptr<iota_t> i) {
  if (!IsKind<std::shared_ptr<donk::assoc_list_t>>()) {
    data_ = std::make_shared<donk::assoc_list_t>();
  }

  std::get<std::shared_ptr<donk::assoc_list_t>>(data_)->operator+=(i);

  return *this;
}

var_t& var_t::operator+=(path_t p) {
  if (!IsKind<std::shared_ptr<donk::assoc_list_t>>()) {
    data_ = std::make_shared<donk::assoc_list_t>();
  }

  std::get<std::shared_ptr<donk::assoc_list_t>>(data_)->operator+=(p);

  return *this;
}

var_t& var_t::operator=(int i) {
  data_.emplace<int>(i);
  return *this;
}

var_t& var_t::operator=(std::string s) {
  data_.emplace<std::string>(s);
  return *this;
}

var_t& var_t::operator=(float f) {
  data_.emplace<float>(f);
  return *this;
}

var_t& var_t::operator=(std::shared_ptr<var_t> v) {
  data_ = v->data_;
  return *this;
}

var_t& var_t::operator=(var_t v) {
  data_ = v.data_;
  return *this;
}

var_t& var_t::operator=(donk::uuid_t uuid) {
  data_.emplace<donk::uuid_t>(uuid);
  return *this;
}

var_t& var_t::operator=(donk::prefab_t preset) {
  data_.emplace<donk::prefab_t>(preset);
  return *this;
}

}  // namespace donk

bool operator<(const std::shared_ptr<donk::var_t> v, const int i) {
  if (v->IsKind<int>()) {
    return v->get_int() < i;
  }
  throw std::runtime_error("incompatible comparison types");
}

bool operator<(const int i, const std::shared_ptr<donk::var_t> v) {
  if (v->IsKind<int>()) {
    return i < v->get_int();
  }
  throw std::runtime_error("incompatible comparison types");
}
