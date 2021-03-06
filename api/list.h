// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_declaration.h.tmpl
// Filename:    list.h

#ifndef __DONK_API_LIST_H__
#define __DONK_API_LIST_H__

#include "cppcoro/generator.hpp"
#include "donk/core/iota.h"
#include "donk/core/procs.h"

namespace donk::api::list {

class list_coretype : public virtual donk::iota_t {
 public:
  ~list_coretype() {}
  void InternalCoreRegister() override;

  list_coretype(donk::path_t path);

  friend class donk::iota_t;
};

donk::running_proc Add(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Copy(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Cut(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Find(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Insert(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Join(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Remove(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Splice(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Swap(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);

}  // namespace donk::api::list

#endif  // __DONK_API_LIST_H__
