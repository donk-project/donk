// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_declaration.h.tmpl
// Filename:    datum/atom/movable/mob.h

#ifndef __DONK_API_DATUM_ATOM_MOVABLE_MOB_H__
#define __DONK_API_DATUM_ATOM_MOVABLE_MOB_H__

#include "cppcoro/generator.hpp"
#include "donk/api/datum/atom/movable.h"
#include "donk/core/procs.h"

namespace donk::api::datum::atom::movable::mob {

class mob_coretype : public donk::api::datum::atom::movable::movable_coretype {
 public:
  ~mob_coretype() {}
  void InternalCoreRegister() override;

  mob_coretype(donk::path_t path);

  friend class donk::iota_t;
};

donk::running_proc Login(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Logout(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);

}  // namespace donk::api::datum::atom::movable::mob

#endif  // __DONK_API_DATUM_ATOM_MOVABLE_MOB_H__
