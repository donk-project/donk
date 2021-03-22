// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_definition_cc.tmpl
// Filename:    sound.cc
#include "donk/api/sound.h"

#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"

namespace donk::api::sound {
sound_coretype::sound_coretype(donk::path_t path) : donk::iota_t(path) {
  InternalCoreRegister();
}

donk::running_proc New(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/sound/proc/New");
}

void sound_coretype::InternalCoreRegister() {
  RegisterProc("New", donk::api::sound::New);
  RegisterVar("channel");
  RegisterVar("echo");
  donk::var_t donk_value_var__environment = -1;
  RegisterVar("environment", donk_value_var__environment);
  donk::var_t donk_value_var__falloff = 1;
  RegisterVar("falloff", donk_value_var__falloff);
  RegisterVar("file");
  donk::var_t donk_value_var__frequency = 0;
  RegisterVar("frequency", donk_value_var__frequency);
  donk::var_t donk_value_var__len = 0;
  RegisterVar("len", donk_value_var__len);
  donk::var_t donk_value_var__offset = 0;
  RegisterVar("offset", donk_value_var__offset);
  donk::var_t donk_value_var__pan = 0;
  RegisterVar("pan", donk_value_var__pan);
  donk::var_t donk_value_var__priority = 0;
  RegisterVar("priority", donk_value_var__priority);
  RegisterVar("repeat");
  donk::var_t donk_value_var__status = 0;
  RegisterVar("status", donk_value_var__status);
  donk::var_t donk_value_var__volume = 100;
  RegisterVar("volume", donk_value_var__volume);
  RegisterVar("wait");
  donk::var_t donk_value_var__x = 0;
  RegisterVar("x", donk_value_var__x);
  donk::var_t donk_value_var__y = 0;
  RegisterVar("y", donk_value_var__y);
  donk::var_t donk_value_var__z = 0;
  RegisterVar("z", donk_value_var__z);
}

}  // namespace donk::api::sound