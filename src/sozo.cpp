//============================================================================
// Name        : sozo.cpp
// Author      : haruki
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <fmt/core.h>
#include <stdio.h>

#include "motors/Motor.hpp"
#include "util/console_util.hpp"

using namespace std;

int main() {
  const int PERIOD = 10000000;

  Motor left_m("P9_14", "P8_26", "P9_12");
  Motor right_m("P9_22", "P8_18", "P8_16");

  left_m.run_pwm(PERIOD, PERIOD / 2, DRIVE_MODE::FORWARD);
  right_m.run_pwm(PERIOD, PERIOD / 2, DRIVE_MODE::FORWARD);

  fmt::print("running!\n");

  while (true) {
    if (utils::kbhit()) {
      if (getchar() == 'q')
        break;
    }
  }
  return 0;
}
