// Donk Project
// Copyright (c) 2021 Warriorstar Orion <orion@snowfrost.garden>
// SPDX-License-Identifier: MIT
#include "runner.h"

int main(int argc, char* argv[]) {
  donk::runner::Runner runner(argv[0]);
  runner.CallMain();
  return 0;
}
