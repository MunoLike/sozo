#include <thread>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "linetrace/linetrace.hpp"
#include "supersonic/supersonic.hpp"
#include "util/console_util.hpp"
#include "motors/Motor.hpp"

#define PERIOD (10000000)

void turn_control() {
  int flag = 0, randam;

  while (flag == 1) {
    if (line_sensors[0] == 0 || line_sensors[3] == 0) {
      usleep(100000);
      flag = 1;
    }
  }

  int turn_flag[3];  //[0]左  [1]中央  [2]右
  turn_flag[0] = 0;
  turn_flag[1] = 0;
  turn_flag[2] = 0;

  if (line_sensors[3] == 0) {
    turn_flag[0] = 1;
  }
  if (line_sensors[0] == 0) {
    turn_flag[2] = 1;
  }
  if (distance_front <= 50) {
    turn_flag[1] = 1;
  }

  randam = rand() % 3;
  flag = 0;
  while (flag == 1) {
    if (line_sensors[randam] == 0) {
      randam = rand() % 3;
    } else {
      flag = 1;
    }
  }

  flag = 0;
  while (flag == 1) {

  }
}

int main() {
  Motor left_m("P9_14", "P8_26", "P9_12");
  Motor right_m("P9_22", "P8_18", "P8_16");

  init_linetrace();
  init_supersonic();

//pthread_t handle;
//pthread_create(&handle, NULL, read_supersonic, NULL);
  std::thread th(read_supersonic);

  while (1) {
    read_linetrace();
    printf("%d %d %d %d %d[mm]\r", line_sensors[0], line_sensors[1],
           line_sensors[2], line_sensors[3], distance_front.load());

    if (line_sensors[1] == 1) {
      left_m.run_pwm(PERIOD, PERIOD * 0.7, DRIVE_MODE::FORWARD);
      right_m.run_pwm(PERIOD, PERIOD * 0.9, DRIVE_MODE::FORWARD);
    } else if (line_sensors[2] == 1) {
      left_m.run_pwm(PERIOD, PERIOD * 0.9, DRIVE_MODE::FORWARD);
      right_m.run_pwm(PERIOD, PERIOD * 0.7, DRIVE_MODE::FORWARD);
    } else {
      left_m.run_pwm(PERIOD, PERIOD * 0.7, DRIVE_MODE::FORWARD);
      right_m.run_pwm(PERIOD, PERIOD * 0.7, DRIVE_MODE::FORWARD);
    }

    if (utils::kbhit() == 'q') {
      break;
    }
  }
  return 0;
}
