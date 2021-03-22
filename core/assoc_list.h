// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __DONK_CORE_ASSOC_LIST_H__
#define __DONK_CORE_ASSOC_LIST_H__

#include <deque>
#include <initializer_list>
#include <map>
#include <string>

#include "donk/core/vars.h"

namespace donk {

using assoc_list_t_iterator =
    std::deque<std::shared_ptr<donk::var_t>>::iterator;

// Many arguments are represented in DM as assoc-lists, such as `list_reagents =
// list("nutriment" = 6, "vitamin" = 2)`. assoc-lists are simple key-value
// stores which only allow strings as keys, and var_ts as values.
class assoc_list_t {
 public:
  assoc_list_t();
  assoc_list_t(std::initializer_list<int>);

  int size() const;
  bool empty() const;

  std::shared_ptr<var_t> front() const;
  void pop_front();

  std::shared_ptr<var_t> v(std::string s);
  std::shared_ptr<var_t> v(int i);

  void insert_by_name(std::string s, std::shared_ptr<var_t> v);
  void insert(std::shared_ptr<var_t> v);

  assoc_list_t& operator+=(std::shared_ptr<var_t> var);
  assoc_list_t& operator+=(std::shared_ptr<iota_t> iota);
  assoc_list_t& operator+=(donk::path_t path);

  // Iterate over vars by index. Note this doesn't supply var names
  assoc_list_t_iterator begin();
  assoc_list_t_iterator end();

  std::map<std::string, std::shared_ptr<var_t>>& vars_by_name() {
    return vars_by_name_;
  }

 private:
  std::map<std::string, std::shared_ptr<var_t>> vars_by_name_;
  std::deque<std::shared_ptr<var_t>> vars_by_index_;

  friend class var_table_t;
  friend class var_t;
};

}  // namespace donk

#endif  // __DONK_CORE_ASSOC_LIST_H__
