// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_definition_cc.tmpl
// Filename:    exception.cc
#include "donk/api/exception.h"

#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"

namespace donk::api::exception {
exception_coretype::exception_coretype(donk::path_t path) : donk::iota_t(path) {
  InternalCoreRegister();
}

donk::running_proc New(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/exception/proc/New");
}

void exception_coretype::InternalCoreRegister() {
  RegisterProc("New", donk::api::exception::New);
  RegisterVar("desc");
  RegisterVar("file");
  RegisterVar("line");
  RegisterVar("name");
}

}  // namespace donk::api::exception