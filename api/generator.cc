// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_definition_cc.tmpl
// Filename:    generator.cc
#include "donk/api/generator.h"

#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"

namespace donk::api::generator {
generator_coretype::generator_coretype(donk::path_t path) : donk::iota_t(path) {
  InternalCoreRegister();
}

donk::running_proc Rand(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/generator/proc/Rand");
}

donk::running_proc Turn(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/generator/proc/Turn");
}

void generator_coretype::InternalCoreRegister() {
  RegisterProc("Rand", donk::api::generator::Rand);
  RegisterProc("Turn", donk::api::generator::Turn);
}

}  // namespace donk::api::generator