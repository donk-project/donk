// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/mapping/tgm_parser.h"

#include <string>
#include <vector>

#include "donk/core/preset.h"
#include "donk/mapping/tgm_parser.h"

namespace donk {
namespace mapping {

peg::parser Parser() {
  peg::parser parser(R"(
MAP           <- HEADER KEY_DECLS ROW_DECLS
HEADER        <- '//MAP CONVERTED BY dmm2tgm.py THIS HEADER COMMENT PREVENTS RECONVERSION, DO NOT REMOVE'
KEY_DECLS     <- KEY_DECL*
ROW_DECLS     <- ROW_DECL*
KEY_DECL      <- '"' KEY_NAME '"' '=' '(' PRESET (',' PRESET)*  ')'
PRESET        <- PATH ( '{' ATTR (';' ATTR)* '}' ) / PATH
ATTR          <- ATTR_NAME '=' ATTR_VAL
ATTR_NAME     <- < [a-zA-Z0-9] [a-zA-Z0-9-_]* > 
ATTR_VAL      <- NUMBER / '"' ATTR_STR_DATA '"'
ATTR_STR_DATA <- < [a-zA-Z0-9] [a-zA-Z0-9-_]* >
KEY_NAME      <- < [a-zA-Z0-9] [a-zA-Z0-9]* > 
PATH          <- < '/' PATH_PART ('/' PATH_PART)* >
PATH_PART     <- < [a-zA-Z0-9] [a-zA-Z0-9-_]* > 
ROW_DECL      <- TRIPLET '=' '{' '"' KEY_NAME* '"' '}'
TRIPLET       <- '(' NUMBER ',' NUMBER ',' NUMBER ')'
NUMBER        <- < [0-9]+ >

%whitespace  <-  [ \t\r\n]*
  )");

  assert((bool)parser == true);

  parser["MAP"] = [](const peg::SemanticValues& sv) {
    map_t map;
    auto parsed_key_decls = std::any_cast<std::vector<key_decl_t>>(sv[1]);
    auto parsed_row_decls = std::any_cast<std::vector<row_decl_t>>(sv[2]);
    for (const auto& key_decl : parsed_key_decls) {
      map.presets[key_decl.name] = key_decl.presets;
    }
    for (const auto& row_decl : parsed_row_decls) {
      map.rows.emplace(std::make_pair(row_decl.triplet, row_decl));
    }
    return map;
  };
  parser["KEY_DECLS"] = [](const peg::SemanticValues& sv) {
    std::vector<key_decl_t> keys;
    for (int i = 0; i < sv.size(); i++) {
      keys.push_back(std::any_cast<key_decl_t>(sv[i]));
    }
    return keys;
  };
  parser["KEY_DECL"] = [](const peg::SemanticValues& sv) {
    key_decl_t key_decl(std::any_cast<std::string>(sv[0]));
    for (int i = 1; i < sv.size(); i++) {
      key_decl.presets.push_back(std::any_cast<donk::preset_t>(sv[i]));
    }
    return key_decl;
  };
  parser["ROW_DECLS"] = [](const peg::SemanticValues& sv) {
    std::vector<row_decl_t> rows;
    for (int i = 0; i < sv.size(); i++) {
      rows.push_back(std::any_cast<row_decl_t>(sv[i]));
    }
    return rows;
  };
  parser["PRESET"] = [](const peg::SemanticValues& sv) {
    donk::preset_t preset(std::any_cast<donk::path_t>(sv[0]));
    switch (sv.choice()) {
      case 0:
        for (int i = 1; i < sv.size(); i++) {
          auto pair = std::any_cast<attr_pair>(sv[i]);
          preset.vars().insert_by_name(pair.name,
                                       std::make_shared<var_t>(pair.var));
        }
      default:
        break;
    }
    return preset;
  };
  parser["ATTR"] = [](const peg::SemanticValues& sv) {
    attr_pair pair;
    pair.name = std::any_cast<std::string>(sv[0]);
    pair.var = std::any_cast<donk::var_t>(sv[1]);
    return pair;
  };
  parser["ATTR_NAME"] = [](const peg::SemanticValues& sv) {
    return sv.token();
  };
  parser["ATTR_VAL"] = [](const peg::SemanticValues& sv) {
    switch (sv.choice()) {
      case 0:
        return donk::var_t(std::any_cast<int>(sv[0]));
      default:
        return donk::var_t(std::any_cast<std::string>(sv[0]));
    }
  };
  parser["ATTR_STR_DATA"] = [](const peg::SemanticValues& sv) {
    return sv.token();
  };
  parser["KEY_NAME"] = [](const peg::SemanticValues& sv) { return sv.token(); };
  parser["PATH"] = [](const peg::SemanticValues& sv) {
    return donk::path_t(sv.token());
  };
  parser["PATH_PART"] = [](const peg::SemanticValues& sv) {
    return sv.token();
  };
  parser["ROW_DECL"] = [](const peg::SemanticValues& sv) {
    std::vector<std::string> keys;
    for (int i = 1; i < sv.size(); i++) {
      keys.push_back(std::any_cast<std::string>(sv[i]));
    }
    return row_decl_t(std::any_cast<triplet_t>(sv[0]), keys);
  };

  parser["TRIPLET"] = [](const peg::SemanticValues& sv) {
    return triplet_t(std::any_cast<int>(sv[0]), std::any_cast<int>(sv[1]),
                     std::any_cast<int>(sv[2]));
  };
  parser["NUMBER"] = [](const peg::SemanticValues& sv) {
    return stoi(sv.token(), nullptr, 10);
  };

  return parser;
}

}  // namespace mapping
}  // namespace donk
