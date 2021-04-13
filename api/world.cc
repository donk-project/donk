// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_definition_cc.tmpl
// Filename:    world.cc
#include "donk/api/world.h"

#include <string>

#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"
#include "donk/core/vars.h"
#include "spdlog/spdlog.h"

namespace donk::api::world {
world_coretype::world_coretype(donk::path_t path) : donk::iota_t(path) {
  InternalCoreRegister();
}

donk::running_proc AddCredits(donk::proc_ctxt_t& ctxt,
                              donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/AddCredits");
}

donk::running_proc ClearMedal(donk::proc_ctxt_t& ctxt,
                              donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/ClearMedal");
}

donk::running_proc Del(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/Del");
}

donk::running_proc Error(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/Error");
}

donk::running_proc Export(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/Export");
}

donk::running_proc GetConfig(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/GetConfig");
}

donk::running_proc GetCredits(donk::proc_ctxt_t& ctxt,
                              donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/GetCredits");
}

donk::running_proc GetMedal(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/GetMedal");
}

donk::running_proc GetScores(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/GetScores");
}

donk::running_proc Import(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/Import");
}

donk::running_proc IsBanned(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/IsBanned");
}

donk::running_proc IsSubscribed(donk::proc_ctxt_t& ctxt,
                                donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/IsSubscribed");
}

donk::running_proc New(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/New");
}

donk::running_proc OpenPort(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/OpenPort");
}

donk::running_proc PayCredits(donk::proc_ctxt_t& ctxt,
                              donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/PayCredits");
}

donk::running_proc Profile(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/Profile");
}

donk::running_proc Reboot(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/Reboot");
}

donk::running_proc Repop(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/Repop");
}

donk::running_proc SetConfig(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/SetConfig");
}

donk::running_proc SetMedal(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/SetMedal");
}

donk::running_proc SetScores(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/SetScores");
}

donk::running_proc Topic(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/world/proc/Topic");
}

donk::running_proc DONKAPI_Broadcast(donk::proc_ctxt_t& ctxt,
                                     donk::proc_args_t& args) {
  spdlog::info("{}", args.v(0)->get_string());
  co_return;
}

donk::running_proc DONKAPI_BroadcastLog(donk::proc_ctxt_t& ctxt,
                                        donk::proc_args_t& args) {
  spdlog::info("{}", args.v(0)->get_string());
  co_return;
}

void world_coretype::InternalCoreRegister() {
  RegisterProc("AddCredits", donk::api::world::AddCredits);
  RegisterProc("ClearMedal", donk::api::world::ClearMedal);
  RegisterProc("Del", donk::api::world::Del);
  RegisterProc("Error", donk::api::world::Error);
  RegisterProc("Export", donk::api::world::Export);
  RegisterProc("GetConfig", donk::api::world::GetConfig);
  RegisterProc("GetCredits", donk::api::world::GetCredits);
  RegisterProc("GetMedal", donk::api::world::GetMedal);
  RegisterProc("GetScores", donk::api::world::GetScores);
  RegisterProc("Import", donk::api::world::Import);
  RegisterProc("IsBanned", donk::api::world::IsBanned);
  RegisterProc("IsSubscribed", donk::api::world::IsSubscribed);
  RegisterProc("New", donk::api::world::New);
  RegisterProc("OpenPort", donk::api::world::OpenPort);
  RegisterProc("PayCredits", donk::api::world::PayCredits);
  RegisterProc("Profile", donk::api::world::Profile);
  RegisterProc("Reboot", donk::api::world::Reboot);
  RegisterProc("Repop", donk::api::world::Repop);
  RegisterProc("SetConfig", donk::api::world::SetConfig);
  RegisterProc("SetMedal", donk::api::world::SetMedal);
  RegisterProc("SetScores", donk::api::world::SetScores);
  RegisterProc("Topic", donk::api::world::Topic);
  RegisterProc("DONKAPI_Broadcast", donk::api::world::DONKAPI_Broadcast);
  RegisterProc("DONKAPI_BroadcastLog", donk::api::world::DONKAPI_BroadcastLog);
  RegisterVar("address");
  donk::var_t donk_value_var__area = donk::prefab_t("/datum/atom/area");
  RegisterVar("area", donk_value_var__area);
  donk::var_t donk_value_var__byond_build = 1527;
  RegisterVar("byond_build", donk_value_var__byond_build);
  donk::var_t donk_value_var__byond_version = 513;
  RegisterVar("byond_version", donk_value_var__byond_version);
  donk::var_t donk_value_var__cache_lifespan = 30;
  RegisterVar("cache_lifespan", donk_value_var__cache_lifespan);
  RegisterVar("contents");
  RegisterVar("cpu");
  RegisterVar("executor");
  donk::var_t donk_value_var__fps = 10;
  RegisterVar("fps", donk_value_var__fps);
  donk::var_t donk_value_var__game_state = 0;
  RegisterVar("game_state", donk_value_var__game_state);
  RegisterVar("host");
  RegisterVar("hub");
  RegisterVar("hub_password");
  donk::var_t donk_value_var__icon_size = 32;
  RegisterVar("icon_size", donk_value_var__icon_size);
  RegisterVar("internet_address");
  RegisterVar("log");
  donk::var_t donk_value_var__loop_checks = 1;
  RegisterVar("loop_checks", donk_value_var__loop_checks);
  RegisterVar("map_cpu");
  donk::var_t donk_value_var__map_format = 0;
  RegisterVar("map_format", donk_value_var__map_format);
  RegisterVar("maxx");
  RegisterVar("maxy");
  RegisterVar("maxz");
  donk::var_t donk_value_var__mob = donk::prefab_t("/datum/atom/movable/mob");
  RegisterVar("mob", donk_value_var__mob);
  donk::var_t donk_value_var__name = std::string("byond");
  RegisterVar("name", donk_value_var__name);
  RegisterVar("params");
  RegisterVar("port");
  RegisterVar("reachable");
  RegisterVar("realtime");
  donk::var_t donk_value_var__sleep_offline = 0;
  RegisterVar("sleep_offline", donk_value_var__sleep_offline);
  RegisterVar("status");
  RegisterVar("system_type");
  donk::var_t donk_value_var__tick_lag = 1;
  RegisterVar("tick_lag", donk_value_var__tick_lag);
  RegisterVar("tick_usage");
  RegisterVar("time");
  RegisterVar("timeofday");
  RegisterVar("timezone");
  donk::var_t donk_value_var__turf = donk::prefab_t("/datum/atom/turf");
  RegisterVar("turf", donk_value_var__turf);
  RegisterVar("url");
  donk::var_t donk_value_var__version = 0;
  RegisterVar("version", donk_value_var__version);
  donk::var_t donk_value_var__view = 5;
  RegisterVar("view", donk_value_var__view);
  donk::var_t donk_value_var__visibility = 1;
  RegisterVar("visibility", donk_value_var__visibility);
}

}  // namespace donk::api::world
