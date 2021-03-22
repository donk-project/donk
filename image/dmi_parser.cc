// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "donk/image/dmi_parser.h"

#include <string>
#include <variant>
#include <vector>

namespace donk {
namespace image {

peg::parser Parser() {
  peg::parser parser(R"(
DMI_DATA     <- DMI_HEADER DMI_METADATA STATE_DECLS DMI_FOOTER
DMI_HEADER   <- '# BEGIN DMI'
DMI_FOOTER   <- '# END DMI'
DMI_METADATA <- 'version = 4.0' WIDTH HEIGHT
WIDTH        <- 'width' '=' ( NUMBER )
HEIGHT       <- 'height' '=' ( NUMBER )
STATE_DECLS  <- (STATE_DECL)*
STATE_DECL   <- 'state' '=' '"' STATE_NAME '"' SETTINGS
SETTINGS     <- (SETTING)*
SETTING      <- SETTING_NAME '=' NUMBER / ( FLOAT (',' FLOAT)* )
SETTING_NAME <- 'dirs' / 'frames' / 'rewind' / 'delay'
STATE_NAME   <- < [a-zA-Z0-9] [a-zA-Z0-9-_]* >
FLOAT        <- < [0-9]*.?[0-9]+ >
NUMBER       <- < [0-9]+ >

%whitespace  <-  [ \t\r\n]*
)");

  assert((bool)parser == true);

  parser["DMI_DATA"] = [](const peg::SemanticValues& sv) {
    dmi_data_t data;
    auto size = std::any_cast<std::pair<int, int>>(sv[1]);
    data.icon_width = size.first;
    data.icon_height = size.second;
    data.states = std::any_cast<std::vector<dmi_state_t>>(sv[2]);
    return data;
  };

  parser["DMI_METADATA"] = [](const peg::SemanticValues& sv) {
    return std::pair<int, int>(std::any_cast<int>(sv[0]),
                               std::any_cast<int>(sv[1]));
  };

  parser["WIDTH"] = [](const peg::SemanticValues& sv) {
    return std::any_cast<int>(sv[0]);
  };

  parser["HEIGHT"] = [](const peg::SemanticValues& sv) {
    return std::any_cast<int>(sv[0]);
  };

  parser["STATE_DECLS"] = [](const peg::SemanticValues& sv) {
    std::vector<dmi_state_t> states;
    for (int i = 0; i < sv.size(); i++) {
      states.push_back(std::any_cast<dmi_state_t>(sv[i]));
    }
    return states;
  };

  parser["STATE_DECL"] = [](const peg::SemanticValues& sv) {
    dmi_state_t state;
    state.name = std::any_cast<std::string>(sv[0]);
    auto settings = std::any_cast<std::vector<
        std::pair<std::string, std::variant<int, std::vector<float>>>>>(sv[1]);
    for (int i = 0; i < settings.size(); i++) {
      auto pair = settings[i];
      auto name = std::any_cast<std::string>(pair.first);
      if (name.compare("dirs") == 0) {
        state.dirs = std::get<int>(pair.second);
      } else if (name.compare("frames") == 0) {
        state.frames = std::get<int>(pair.second);
      } else if (name.compare("rewind") == 0) {
        state.rewind = std::get<int>(pair.second);
      } else if (name.compare("delay") == 0) {
        state.delay = std::get<std::vector<float>>(pair.second);
      }
    }
    return state;
  };

  parser["SETTINGS"] = [](const peg::SemanticValues& sv) {
    std::vector<std::pair<std::string, std::variant<int, std::vector<float>>>>
        settings;
    for (int i = 0; i < sv.size(); i++) {
      settings.push_back(
          std::any_cast<
              std::pair<std::string, std::variant<int, std::vector<float>>>>(
              sv[i]));
    }
    return settings;
  };

  parser["SETTING"] = [](const peg::SemanticValues& sv) {
    std::pair<std::string, std::variant<int, std::vector<float>>> setting;
    setting.first = std::any_cast<std::string>(sv[0]);
    switch (sv.choice()) {
      case 0:
        setting.second = std::any_cast<int>(sv[1]);
        break;
      case 1:
        std::vector<float> delays;
        for (int i = 1; i < sv.tokens.size(); i++) {
          delays.push_back(std::any_cast<float>(sv[i]));
        }
        setting.second = delays;
        break;
    }
    return setting;
  };

  parser["SETTING_NAME"] = [](const peg::SemanticValues& sv) {
    return std::any_cast<std::string>(sv.token());
  };

  parser["STATE_NAME"] = [](const peg::SemanticValues& sv) {
    return sv.token();
  };

  parser["FLOAT"] = [](const peg::SemanticValues& sv) {
    return std::stof(std::any_cast<std::string>(sv.token()));
  };

  parser["NUMBER"] = [](const peg::SemanticValues& sv) {
    return stoi(sv.token(), nullptr, 10);
  };

  return parser;
}

}  // namespace image
}  // namespace donk
