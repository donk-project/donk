// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_definition_cc.tmpl
// Filename:    regex.cc
#include "donk/api/regex.h"

#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"

namespace donk::api::regex {
regex_coretype::regex_coretype(donk::path_t path) : donk::iota_t(path) {
  InternalCoreRegister();
}

donk::running_proc Find(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/regex/proc/Find");
}

donk::running_proc Find_char(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/regex/proc/Find_char");
}

donk::running_proc New(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/regex/proc/New");
}

donk::running_proc Replace(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/regex/proc/Replace");
}

donk::running_proc Replace_char(donk::proc_ctxt_t& ctxt,
                                donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/regex/proc/Replace_char");
}

void regex_coretype::InternalCoreRegister() {
  RegisterProc("Find", donk::api::regex::Find);
  RegisterProc("Find_char", donk::api::regex::Find_char);
  RegisterProc("New", donk::api::regex::New);
  RegisterProc("Replace", donk::api::regex::Replace);
  RegisterProc("Replace_char", donk::api::regex::Replace_char);
  RegisterVar("flags");
  RegisterVar("group");
  RegisterVar("index");
  RegisterVar("match");
  RegisterVar("name");
  RegisterVar("next");
  RegisterVar("text");
}

}  // namespace donk::api::regex