// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __SNOWFROST_DONK_CORE_VARS_H__
#define __SNOWFROST_DONK_CORE_VARS_H__

#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "donk/core/path.h"
#include "donk/core/prefab.h"
#include "donk/core/resource.h"
#include "fmt/format.h"
#include "fmt/ostream.h"

namespace donk {

class preset_t;

using entity_id = unsigned int;

namespace networking {
class ProtoSerializers;
}

class assoc_list_t;
class iota_t;
class preset_t;
class proc_args_t;
class proc_ctxt_t;
class var_t;

// var_t objects are flexible container types which can hold nearly anything
// that the interpreter can create.
class var_t {
 public:
  var_t();
  var_t(int);
  var_t(float);
  var_t(resource_t);
  var_t(std::string);
  var_t(donk::path_t);
  var_t(donk::prefab_t);
  var_t(std::shared_ptr<var_t>);
  var_t(std::shared_ptr<iota_t>);
  var_t(const var_t&);
  var_t(assoc_list_t);

  // Un-templated getters for the internal variant type. These are used to keep
  // codegen complexity low.

  int get_int();
  float get_float();
  std::string get_string();
  entity_id get_entity_id();
  path_t get_path();
  prefab_t get_prefab();
  std::shared_ptr<resource_t> get_resource();
  std::shared_ptr<iota_t> get_iota();
  std::shared_ptr<assoc_list_t> get_list();

  template <typename Kind>
  bool IsKind() {
    return std::holds_alternative<Kind>(data_);
  }

  static std::shared_ptr<var_t> str(std::string s) {
    auto v = std::make_shared<var_t>(s);
    return v;
  }

  // Convenience call-through for a var in an iota-filled var.
  std::shared_ptr<var_t> v(const std::string&);

  var_t& operator=(float);
  var_t& operator=(int);
  var_t& operator=(prefab_t);
  var_t& operator=(std::shared_ptr<iota_t>);
  var_t& operator=(std::shared_ptr<resource_t>);
  var_t& operator=(std::shared_ptr<var_t>);
  var_t& operator=(std::string);
  var_t& operator=(entity_id);
  var_t& operator=(var_t);
  var_t& operator=(path_t);

  var_t& operator+=(std::string);
  var_t& operator+=(std::shared_ptr<iota_t>);
  var_t& operator+=(std::shared_ptr<var_t>);
  var_t& operator+=(path_t);

  std::shared_ptr<var_t> operator-(std::shared_ptr<var_t>);
  std::shared_ptr<var_t> operator*(std::shared_ptr<var_t>);

  std::variant<int, float, std::string, entity_id, std::shared_ptr<iota_t>,
               std::shared_ptr<resource_t>, prefab_t,
               std::shared_ptr<assoc_list_t>, path_t>
      data_;

  friend class var_table_t;
};

class var_table_t {
 public:
  var_table_t()
      : vars_(
            std::make_shared<std::map<std::string, std::shared_ptr<var_t>>>()) {
  }

  void Register(std::string, var_t&);
  void Register(std::string);
  void RegisterPreset(preset_t&);
  std::shared_ptr<var_t> Lookup(std::string);

  template <typename Kind>
  bool HasTypedVar(std::string s) const {
    if (vars_ == nullptr) {
      spdlog::critical("HasTypedVar({}) on null vars_", s);
    }
    auto result = vars_->find(s);
    if (result != vars_->end()) {
      return std::holds_alternative<Kind>(result->second->data_);
    }
    return false;
  }

  bool HasVar(std::string s) const {
    auto result = vars_->find(s);
    if (result != vars_->end()) {
      return true;
    }
    return false;
  }

  std::string const_string(std::string s) const {
    auto result = vars_->find(s);
    if (result != vars_->end()) {
      return std::get<std::string>(result->second->data_);
    }
    return "";
  }

  int size() const { return vars_->size(); }

  std::shared_ptr<std::map<std::string, std::shared_ptr<var_t>>> vars_;
  friend class donk::networking::ProtoSerializers;
};

}  // namespace donk

namespace fmt {

template <>
struct fmt::formatter<::donk::var_table_t> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(donk::var_table_t const& p, FormatContext& ctx) {
    return fmt::format_to(ctx.out(), "var_table_t<@{}, {}>", fmt::ptr(&p),
                          p.vars_->size());
  }
};

template <>
struct fmt::formatter<donk::prefab_t> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(donk::prefab_t const& p, FormatContext& ctx) {
    return fmt::format_to(ctx.out(), "prefab_t<{}>", p.path().fq());
  }
};

template <>
struct fmt::formatter<::donk::var_t> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }
  template <typename FormatContext>
  class VarFormatterVisiter {
   public:
    VarFormatterVisiter(FormatContext& ctx) : ctx_(ctx) {}
    void operator()(const int& i) { fmt::format_to(ctx_.out(), "{}i", i); }
    void operator()(const float& f) { fmt::format_to(ctx_.out(), "{}f", f); }
    void operator()(const std::string& s) {
      fmt::format_to(ctx_.out(), "{}", s);
    }
    void operator()(const std::shared_ptr<::donk::iota_t>& i) {
      fmt::format_to(ctx_.out(), "{}", *i);
    }
    void operator()(const donk::entity_id& entity_id_) {
      fmt::format_to(ctx_.out(), "entity_id/{}", entity_id_);
    }
    void operator()(const donk::prefab_t& prefab) {
      fmt::format_to(ctx_.out(), "{}", prefab);
    }
    void operator()(const std::shared_ptr<::donk::resource_t>& resource) {
      fmt::format_to(ctx_.out(), "{}", resource->name());
    }
    void operator()(const std::shared_ptr<::donk::assoc_list_t>&) {
      fmt::format_to(ctx_.out(), "assoc_list_t<>");
    }
    void operator()(const donk::path_t& p) {
      fmt::format_to(ctx_.out(), "{}", p);
    }
    FormatContext& ctx_;
  };

  template <typename FormatContext>
  auto format(::donk::var_t const& var, FormatContext& ctx) {
    std::visit(VarFormatterVisiter{ctx}, var.data_);
    return fmt::format_to(ctx.out(), "");
  }
};

}  // namespace fmt

bool operator<(const std::shared_ptr<donk::var_t> v, const int i);
bool operator<(const int i, const std::shared_ptr<donk::var_t> v);

#endif  // __SNOWFROST_DONK_CORE_VARS_H__
