// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/core/assoc_list.h"

#include <initializer_list>
#include <string>

#include "donk/core/vars.h"

namespace donk {

assoc_list_t::assoc_list_t() {}

assoc_list_t::assoc_list_t(std::initializer_list<int> ints) {
  for (int i : ints) {
    insert(std::make_shared<var_t>(i));
  }
}

int assoc_list_t::size() const { return vars_by_index_.size(); }

bool assoc_list_t::empty() const { return vars_by_index_.empty(); }

std::shared_ptr<var_t> assoc_list_t::front() const {
  return vars_by_index_.front();
}

void assoc_list_t::pop_front() { vars_by_index_.pop_front(); }

std::shared_ptr<var_t> assoc_list_t::v(std::string s) {
  auto pair = vars_by_name_.find(s);
  if (pair != vars_by_name_.end()) {
    return pair->second;
  }
  auto it =
      vars_by_index_.insert(vars_by_index_.end(), std::make_shared<var_t>());
  vars_by_name_[s] = *it;
  return *it;
}

std::shared_ptr<var_t> assoc_list_t::v(int i) {
  if (vars_by_index_.empty() || i >= vars_by_index_.size()) {
    throw std::runtime_error(
        fmt::format("assoc list has no element at index {}", i));
  }
  return vars_by_index_[i];
}

void assoc_list_t::insert_by_name(std::string s, std::shared_ptr<var_t> v) {
  auto pair = vars_by_name_.find(s);
  if (pair != vars_by_name_.end()) {
    pair->second = v;
  }
  vars_by_index_.insert(vars_by_index_.end(), v);
  vars_by_name_[s] = v;
}

void assoc_list_t::insert(std::shared_ptr<var_t> v) {
  vars_by_index_.insert(vars_by_index_.end(), v);
}

assoc_list_t& assoc_list_t::operator+=(std::shared_ptr<var_t> var) {
  vars_by_index_.push_back(var);
  return *this;
}

assoc_list_t& assoc_list_t::operator+=(std::shared_ptr<iota_t> iota) {
  vars_by_index_.push_back(std::make_shared<var_t>(iota));
  return *this;
}

assoc_list_t& assoc_list_t::operator+=(donk::path_t path) {
  vars_by_index_.push_back(std::make_shared<var_t>(path));
  return *this;
}

assoc_list_t_iterator assoc_list_t::begin() { return vars_by_index_.begin(); }
assoc_list_t_iterator assoc_list_t::end() { return vars_by_index_.end(); }

}  // namespace donk
