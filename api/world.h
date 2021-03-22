// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_declaration.h.tmpl
// Filename:    world.h

#ifndef __DONK_API_WORLD_H__
#define __DONK_API_WORLD_H__

#include <deque>
#include <map>
#include <string>

#include "cppcoro/generator.hpp"
#include "donk/api/client.h"
#include "donk/core/iota.h"
#include "donk/core/procs.h"

namespace donk::api::world {

class world_coretype : public virtual donk::iota_t {
 public:
  ~world_coretype() {}
  void InternalCoreRegister() override;

  std::map<std::string, std::shared_ptr<donk::api::client::client_coretype>>
      clients_by_username_;

  std::deque<std::string> internal_broadcast_log_;
  std::string PopBroadcast() {
    auto x = internal_broadcast_log_.front();
    internal_broadcast_log_.pop_front();
    return x;
  }

  world_coretype(donk::path_t path);

  friend class donk::iota_t;
};

donk::running_proc AddCredits(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc ClearMedal(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Del(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Error(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Export(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc GetConfig(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc GetCredits(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc GetMedal(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc GetScores(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Import(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc IsBanned(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc IsSubscribed(donk::proc_ctxt_t& ctxt,
                                donk::proc_args_t& args);
donk::running_proc New(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc OpenPort(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc PayCredits(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Profile(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Reboot(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Repop(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc SetConfig(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc SetMedal(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc SetScores(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc Topic(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args);
donk::running_proc DONKAPI_Broadcast(donk::proc_ctxt_t& ctxt,
                                     donk::proc_args_t& args);
donk::running_proc DONKAPI_BroadcastLog(donk::proc_ctxt_t& ctxt,
                                        donk::proc_args_t& args);

}  // namespace donk::api::world

#endif  // __DONK_API_WORLD_H__