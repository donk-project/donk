// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_definition_cc.tmpl
// Filename:    datum/atom/turf.cc
#include "donk/api/datum/atom/turf.h"

#include "donk/api/datum/atom.h"
#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"
#include "donk/core/vars.h"

namespace donk::api::datum::atom::turf {
turf_coretype::turf_coretype(donk::path_t path)
    : donk::iota_t(path), donk::api::datum::atom::atom_coretype(path) {
  InternalCoreRegister();
}

void turf_coretype::InternalCoreRegister() {
  donk::var_t donk_value_var__layer = 2;
  RegisterVar("layer", donk_value_var__layer);
  donk::var_t donk_value_var__parent_type = donk::prefab_t("/datum/atom");
  RegisterVar("parent_type", donk_value_var__parent_type);
  RegisterVar("vis_contents");
  RegisterVar("vis_locs");
}

}  // namespace donk::api::datum::atom::turf