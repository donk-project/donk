// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#ifndef __SNOWFROST_DONK_CORE_IOTA_H__
#define __SNOWFROST_DONK_CORE_IOTA_H__

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "donk/core/path.h"
#include "donk/core/procs.h"
#include "donk/core/vars.h"
#include "fmt/ostream.h"
#include "spdlog/spdlog.h"

namespace pollux::client::states {
class ClientRoom;
}

namespace donk {
class iota_t;

namespace ecs {
class EcsManager;
}

namespace mapping {
class MapRoster;
}

// iota_t is the base of the Donk object hierarchy. It requires a known
// DM object-tree path and provides both var_table_t and proc_table_t
// fields.
//
// It contains all methods related to reflection and method invocation
// which the DM interpreter relies on.
class iota_t {
 public:
  iota_t(path_t);
  virtual ~iota_t() {}

  // Overridden by all sub-types but required implementation here since
  // generic 'iota' instances are constructed regularly
  virtual void InternalCoreRegister();

  const path_t GetPath() const;
  const path_t ParentType() const;

  // `v` methods are variable retrievals.
  std::shared_ptr<var_t> v(const std::string& name);

  template <typename Kind>
  bool HasTypedVar(std::string name) const {
    return var_table_.HasTypedVar<Kind>(name);
  }

  bool HasProc(std::string name) const { return proc_table_.HasProc(name); }

  void RegisterProc(std::string, transpiled_proc);
  void ProcSettings(std::string, proc_settings_t);
  void ProcInput(std::string, proc_input_t);
  void RegisterVar(const std::string&, var_t&);
  void RegisterVar(const std::string&);
  void Apply(preset_t preset);

  var_table_t& var_table() { return var_table_; }
  proc_table_t& proc_table() { return proc_table_; }
  std::string var_const_string(std::string s) const {
    return var_table_.const_string(s);
  }

  donk::entity_id entity_id_ = 0;

  friend std::ostream& operator<<(std::ostream& out, donk::iota_t* iota) {
    if (iota->var_table_.HasTypedVar<std::string>("key")) {
      return out << iota->v("key")->get_string();
    } else {
      return out << "UNKNOWN KEY";
    }
  }

  friend std::ostream& operator<<(std::ostream& out, const donk::iota_t& iota) {
    if (iota.var_table_.HasTypedVar<std::string>("key")) {
      return out << iota.var_const_string("key");
    } else {
      return out << "UNKNOWN KEY";
    }
  }

 private:
  const path_t path_;
  proc_table_t proc_table_;
  var_table_t var_table_;

  friend class donk::ecs::EcsManager;
  friend class pollux::client::states::ClientRoom;
};

}  // namespace donk

#endif  // __SNOWFROST_DONK_CORE_IOTA_H__
