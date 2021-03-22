// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_definition_cc.tmpl
// Filename:    icon.cc
#include "donk/api/icon.h"

#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"

namespace donk::api::icon {
icon_coretype::icon_coretype(donk::path_t path) : donk::iota_t(path) {
  InternalCoreRegister();
}

donk::running_proc Blend(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/Blend");
}

donk::running_proc Crop(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/Crop");
}

donk::running_proc DrawBox(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/DrawBox");
}

donk::running_proc Flip(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/Flip");
}

donk::running_proc GetPixel(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/GetPixel");
}

donk::running_proc Height(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/Height");
}

donk::running_proc IconStates(donk::proc_ctxt_t& ctxt,
                              donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/IconStates");
}

donk::running_proc Insert(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/Insert");
}

donk::running_proc MapColors(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/MapColors");
}

donk::running_proc New(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/New");
}

donk::running_proc Scale(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/Scale");
}

donk::running_proc SetIntensity(donk::proc_ctxt_t& ctxt,
                                donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/SetIntensity");
}

donk::running_proc Shift(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/Shift");
}

donk::running_proc SwapColor(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/SwapColor");
}

donk::running_proc Turn(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/Turn");
}

donk::running_proc Width(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/icon/proc/Width");
}

void icon_coretype::InternalCoreRegister() {
  RegisterProc("Blend", donk::api::icon::Blend);
  RegisterProc("Crop", donk::api::icon::Crop);
  RegisterProc("DrawBox", donk::api::icon::DrawBox);
  RegisterProc("Flip", donk::api::icon::Flip);
  RegisterProc("GetPixel", donk::api::icon::GetPixel);
  RegisterProc("Height", donk::api::icon::Height);
  RegisterProc("IconStates", donk::api::icon::IconStates);
  RegisterProc("Insert", donk::api::icon::Insert);
  RegisterProc("MapColors", donk::api::icon::MapColors);
  RegisterProc("New", donk::api::icon::New);
  RegisterProc("Scale", donk::api::icon::Scale);
  RegisterProc("SetIntensity", donk::api::icon::SetIntensity);
  RegisterProc("Shift", donk::api::icon::Shift);
  RegisterProc("SwapColor", donk::api::icon::SwapColor);
  RegisterProc("Turn", donk::api::icon::Turn);
  RegisterProc("Width", donk::api::icon::Width);
}

}  // namespace donk::api::icon