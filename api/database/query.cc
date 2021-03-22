// Generated by Donk Transpiler. Changes may be overwritten.
// Template:    transformer_definition_cc.tmpl
// Filename:    database/query.cc
#include "donk/api/database/query.h"

#include "donk/api/database.h"
#include "donk/core/iota.h"
#include "donk/core/path.h"
#include "donk/core/procs.h"

namespace donk::api::database::query {
query_coretype::query_coretype(donk::path_t path)
    : donk::iota_t(path), donk::api::database::database_coretype(path) {
  InternalCoreRegister();
}

donk::running_proc Add(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/Add");
}

donk::running_proc Close(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/Close");
}

donk::running_proc Columns(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/Columns");
}

donk::running_proc Error(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/Error");
}

donk::running_proc ErrorMsg(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/ErrorMsg");
}

donk::running_proc Execute(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/Execute");
}

donk::running_proc GetColumn(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/GetColumn");
}

donk::running_proc GetRowData(donk::proc_ctxt_t& ctxt,
                              donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/GetRowData");
}

donk::running_proc New(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/New");
}

donk::running_proc NextRow(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/NextRow");
}

donk::running_proc Reset(donk::proc_ctxt_t& ctxt, donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/Reset");
}

donk::running_proc RowsAffected(donk::proc_ctxt_t& ctxt,
                                donk::proc_args_t& args) {
  co_yield ctxt.Unimplemented("/database/query/proc/RowsAffected");
}

void query_coretype::InternalCoreRegister() {
  RegisterProc("Add", donk::api::database::query::Add);
  RegisterProc("Close", donk::api::database::query::Close);
  RegisterProc("Columns", donk::api::database::query::Columns);
  RegisterProc("Error", donk::api::database::query::Error);
  RegisterProc("ErrorMsg", donk::api::database::query::ErrorMsg);
  RegisterProc("Execute", donk::api::database::query::Execute);
  RegisterProc("GetColumn", donk::api::database::query::GetColumn);
  RegisterProc("GetRowData", donk::api::database::query::GetRowData);
  RegisterProc("New", donk::api::database::query::New);
  RegisterProc("NextRow", donk::api::database::query::NextRow);
  RegisterProc("Reset", donk::api::database::query::Reset);
  RegisterProc("RowsAffected", donk::api::database::query::RowsAffected);
}

}  // namespace donk::api::database::query