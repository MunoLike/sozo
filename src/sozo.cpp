#include <thread>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "linetrace/linetrace.hpp"
#include "supersonic/supersonic.hpp"
#include "util/console_util.hpp"
#include "motors/Motor.hpp"
//#include "soket/client.hpp"

#define PERIOD (10000000)
Motor left_m("P9_14", "P8_26", "P9_12");
Motor right_m("P9_22", "P8_18", "P8_16");

int turn_left() {
  left_m.run_pwm(PERIOD, PERIOD * 0.3, DRIVE_MODE::BACKWARD);
  right_m.run_pwm(PERIOD, PERIOD * 0.3, DRIVE_MODE::FORWARD);
  while (1) {
    if (line_sensors[3] == 0) {
      while (1) {
        if (line_sensors[3] == 1)
          while (1) {
            if (line_sensors[2] == 1) {
              return 0;
            }
          }
      }
    }
  }
}

int turn_right() {
  left_m.run_pwm(PERIOD, PERIOD * 0.3, DRIVE_MODE::FORWARD);
  right_m.run_pwm(PERIOD, PERIOD * 0.3, DRIVE_MODE::BACKWARD);
  while (1) {
    if (line_sensors[0] == 0) {
      while (1) {
        if (line_sensors[0] == 1)
          while (1) {
            if (line_sensors[1] == 1) {
              return 0;
            }
          }
      }
    }
  }
}

int turn_control() {

  if (line_sensors[0] == 0 || line_sensors[3] == 0) {
    usleep(10000);
  } else {
    return 0;
  }

  if (line_sensors[3] == 0) {
    usleep(100000);
    turn_left();
  } else if (line_sensors[0] == 0) {
    usleep(100000);
    turn_right();
  }
}

int main() {

  //実行開始用スイッチ
  GPIO::GPIO run_sw("P8_15");

  init_linetrace();
  init_supersonic();

  std::thread th(read_supersonic);

  while (!run_sw.getValue())
    ;
  sleep(1);
  while (1) {
    read_linetrace();
    printf("%d %d %d %d %d[mm]\r", line_sensors[0], line_sensors[1],
           line_sensors[2], line_sensors[3], distance_front.load());

    if (line_sensors[1] == 1) {
      left_m.run_pwm(PERIOD, PERIOD * 0.15, DRIVE_MODE::FORWARD);
      right_m.run_pwm(PERIOD, PERIOD * 0.2, DRIVE_MODE::FORWARD);
    } else if (line_sensors[2] == 1) {
      left_m.run_pwm(PERIOD, PERIOD * 0.2, DRIVE_MODE::FORWARD);
      right_m.run_pwm(PERIOD, PERIOD * 0.15, DRIVE_MODE::FORWARD);
    } else {
      left_m.run_pwm(PERIOD, PERIOD * 0.2, DRIVE_MODE::FORWARD);
      right_m.run_pwm(PERIOD, PERIOD * 0.2, DRIVE_MODE::FORWARD);
    }

    //turn_control();

    if (utils::kbhit() == 'q') {
      break;
    }
  }
  return 0;
}
