// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_declaration.h.tmpl
// Filename:    datum/atom/movable/obj.h

#ifndef __DONK_API_DATUM_ATOM_MOVABLE_OBJ_H__
#define __DONK_API_DATUM_ATOM_MOVABLE_OBJ_H__

#include "donk/api/datum/atom/movable.h"
#include "donk/core/procs.h"

namespace donk::api::datum::atom::movable::obj {

class obj_coretype : public donk::api::datum::atom::movable::movable_coretype {
 public:
  ~obj_coretype() {}
  void InternalCoreRegister() override;

  obj_coretype(donk::path_t path);

  friend class donk::iota_t;
};

}  // namespace donk::api::datum::atom::movable::obj

#endif  // __DONK_API_DATUM_ATOM_MOVABLE_OBJ_H__
