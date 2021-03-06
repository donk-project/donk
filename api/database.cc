// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_definition_cc.tmpl
// Filename:    database.cc
#include "donk/api/database.h"

#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"

namespace donk::api::database {
database_coretype::database_coretype(donk::path_t path) : donk::iota_t(path) {
  InternalCoreRegister();
}

donk::running_proc Close(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/proc/Close");
}

donk::running_proc Error(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/proc/Error");
}

donk::running_proc ErrorMsg(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/proc/ErrorMsg");
}

donk::running_proc New(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/proc/New");
}

donk::running_proc Open(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/proc/Open");
}

void database_coretype::InternalCoreRegister() {
  RegisterProc("Close", donk::api::database::Close);
  RegisterProc("Error", donk::api::database::Error);
  RegisterProc("ErrorMsg", donk::api::database::ErrorMsg);
  RegisterProc("New", donk::api::database::New);
  RegisterProc("Open", donk::api::database::Open);
}

}  // namespace donk::api::database