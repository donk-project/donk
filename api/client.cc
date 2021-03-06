// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_definition_cc.tmpl
// Filename:    client.cc
#include "donk/api/client.h"

#include <string>

#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"

namespace donk::api::client {
client_coretype::client_coretype(donk::path_t path) : donk::iota_t(path) {
  InternalCoreRegister();
}

donk::running_proc AllowUpload(donk::proc_ctxt_t& ctxt,
                               donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/AllowUpload");
}

donk::running_proc Center(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Center");
}

donk::running_proc CheckPassport(donk::proc_ctxt_t& ctxt,
                                 donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/CheckPassport");
}

donk::running_proc Click(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Click");
}

donk::running_proc Command(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Command");
}

donk::running_proc DblClick(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/DblClick");
}

donk::running_proc Del(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Del");
}

donk::running_proc East(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/East");
}

donk::running_proc Export(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Export");
}

donk::running_proc Import(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Import");
}

donk::running_proc IsByondMember(donk::proc_ctxt_t& ctxt,
                                 donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/IsByondMember");
}

donk::running_proc MeasureText(donk::proc_ctxt_t& ctxt,
                               donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/MeasureText");
}

donk::running_proc MouseDown(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/MouseDown");
}

donk::running_proc MouseDrag(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/MouseDrag");
}

donk::running_proc MouseDrop(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/MouseDrop");
}

donk::running_proc MouseEntered(donk::proc_ctxt_t& ctxt,
                                donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/MouseEntered");
}

donk::running_proc MouseExited(donk::proc_ctxt_t& ctxt,
                               donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/MouseExited");
}

donk::running_proc MouseMove(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/MouseMove");
}

donk::running_proc MouseUp(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/MouseUp");
}

donk::running_proc MouseWheel(donk::proc_ctxt_t& ctxt,
                              donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/MouseWheel");
}

donk::running_proc Move(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Move");
}

donk::running_proc New(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  if (ctxt.world()->HasTypedVar<donk::prefab_t>("mob")) {
    auto prefab = ctxt.world()->v("mob")->get_prefab();
    *ctxt.src()->v("mob") = ctxt.make(prefab.path().fq());
  } else if (ctxt.world()->HasTypedVar<donk::path_t>("mob")) {
    auto path = ctxt.world()->v("mob")->get_path();
    *ctxt.src()->v("mob") = ctxt.make(path.fq());
  } else {
    *ctxt.src()->v("mob") = ctxt.make("/mob");
  }

  co_yield ctxt.ChildProc(ctxt.src()->v("mob")->get_iota(), "New", {});

  co_return;
}

donk::running_proc North(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/North");
}

donk::running_proc Northeast(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Northeast");
}

donk::running_proc Northwest(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Northwest");
}

donk::running_proc SendPage(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/SendPage");
}

donk::running_proc SoundQuery(donk::proc_ctxt_t& ctxt,
                              donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/SoundQuery");
}

donk::running_proc South(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/South");
}

donk::running_proc Southeast(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Southeast");
}

donk::running_proc Southwest(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Southwest");
}

donk::running_proc Stat(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Stat");
}

donk::running_proc Topic(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/Topic");
}

donk::running_proc West(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/client/proc/West");
}

void client_coretype::InternalCoreRegister() {
  RegisterProc("AllowUpload", donk::api::client::AllowUpload);
  RegisterProc("Center", donk::api::client::Center);
  RegisterProc("CheckPassport", donk::api::client::CheckPassport);
  RegisterProc("Click", donk::api::client::Click);
  RegisterProc("Command", donk::api::client::Command);
  RegisterProc("DblClick", donk::api::client::DblClick);
  RegisterProc("Del", donk::api::client::Del);
  RegisterProc("East", donk::api::client::East);
  RegisterProc("Export", donk::api::client::Export);
  RegisterProc("Import", donk::api::client::Import);
  RegisterProc("IsByondMember", donk::api::client::IsByondMember);
  RegisterProc("MeasureText", donk::api::client::MeasureText);
  RegisterProc("MouseDown", donk::api::client::MouseDown);
  RegisterProc("MouseDrag", donk::api::client::MouseDrag);
  RegisterProc("MouseDrop", donk::api::client::MouseDrop);
  RegisterProc("MouseEntered", donk::api::client::MouseEntered);
  RegisterProc("MouseExited", donk::api::client::MouseExited);
  RegisterProc("MouseMove", donk::api::client::MouseMove);
  RegisterProc("MouseUp", donk::api::client::MouseUp);
  RegisterProc("MouseWheel", donk::api::client::MouseWheel);
  RegisterProc("Move", donk::api::client::Move);
  RegisterProc("New", donk::api::client::New);
  RegisterProc("North", donk::api::client::North);
  RegisterProc("Northeast", donk::api::client::Northeast);
  RegisterProc("Northwest", donk::api::client::Northwest);
  RegisterProc("SendPage", donk::api::client::SendPage);
  RegisterProc("SoundQuery", donk::api::client::SoundQuery);
  RegisterProc("South", donk::api::client::South);
  RegisterProc("Southeast", donk::api::client::Southeast);
  RegisterProc("Southwest", donk::api::client::Southwest);
  RegisterProc("Stat", donk::api::client::Stat);
  RegisterProc("Topic", donk::api::client::Topic);
  RegisterProc("West", donk::api::client::West);
  RegisterVar("CGI");
  RegisterVar("address");
  RegisterVar("authenticate");
  RegisterVar("bound_height");
  RegisterVar("bound_width");
  RegisterVar("bound_x");
  RegisterVar("bound_y");
  RegisterVar("bounds");
  RegisterVar("byond_build");
  RegisterVar("byond_version");
  RegisterVar("ckey");
  RegisterVar("color");
  RegisterVar("command_text");
  RegisterVar("computer_id");
  RegisterVar("connection");
  donk::var_t donk_value_var__control_freak = 0;
  RegisterVar("control_freak", donk_value_var__control_freak);
  donk::var_t donk_value_var__default_verb_category = std::string("Commands");
  RegisterVar("default_verb_category", donk_value_var__default_verb_category);
  donk::var_t donk_value_var__dir = 1;
  RegisterVar("dir", donk_value_var__dir);
  RegisterVar("edge_limit");
  RegisterVar("eye");
  donk::var_t donk_value_var__fps = 0;
  RegisterVar("fps", donk_value_var__fps);
  RegisterVar("gender");
  donk::var_t donk_value_var__glide_size = 0;
  RegisterVar("glide_size", donk_value_var__glide_size);
  RegisterVar("images");
  RegisterVar("inactivity");
  RegisterVar("key");
  RegisterVar("lazy_eye");
  RegisterVar("mob");
  RegisterVar("mouse_pointer_icon");
  RegisterVar("parent_type");
  donk::var_t donk_value_var__perspective = 0;
  RegisterVar("perspective", donk_value_var__perspective);
  donk::var_t donk_value_var__pixel_w = 0;
  RegisterVar("pixel_w", donk_value_var__pixel_w);
  donk::var_t donk_value_var__pixel_x = 0;
  RegisterVar("pixel_x", donk_value_var__pixel_x);
  donk::var_t donk_value_var__pixel_y = 0;
  RegisterVar("pixel_y", donk_value_var__pixel_y);
  donk::var_t donk_value_var__pixel_z = 0;
  RegisterVar("pixel_z", donk_value_var__pixel_z);
  donk::var_t donk_value_var__preload_rsc = 1;
  RegisterVar("preload_rsc", donk_value_var__preload_rsc);
  RegisterVar("screen");
  RegisterVar("script");
  donk::var_t donk_value_var__show_map = 1;
  RegisterVar("show_map", donk_value_var__show_map);
  donk::var_t donk_value_var__show_popup_menus = 1;
  RegisterVar("show_popup_menus", donk_value_var__show_popup_menus);
  donk::var_t donk_value_var__show_verb_panel = 1;
  RegisterVar("show_verb_panel", donk_value_var__show_verb_panel);
  RegisterVar("statobj");
  RegisterVar("statpanel");
  RegisterVar("tag");
  donk::var_t donk_value_var__tick_lag = 0;
  RegisterVar("tick_lag", donk_value_var__tick_lag);
  RegisterVar("timezone");
  RegisterVar("type");
  RegisterVar("verbs");
  RegisterVar("view");
  RegisterVar("virtual_eye");
}

}  // namespace donk::api::client
