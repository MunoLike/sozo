#include <thread>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fmt/core.h>

#include "linetrace/linetrace.hpp"
#include "supersonic/supersonic.hpp"
#include "util/console_util.hpp"
#include "motors/Motor.hpp"
#include "motors/Servo.hpp"

#define PERIOD (10000000)
Motor right_m("P9_14", "P9_12", "P8_26");
Motor left_m("P9_22", "P8_16", "P8_18", 0.65);
Servo waki("P8_13");

char flag = 'N';
float turn = 0.22;
float max = 0.35;
float low = 0.2;

int turn_left() {
  left_m.run_pwm(PERIOD, PERIOD * turn / 0.65, DRIVE_MODE::BACKWARD);
  right_m.run_pwm(PERIOD, PERIOD * turn, DRIVE_MODE::FORWARD);

  while (1) {
    if (line_sensors[0] == 0) {
      break;
    }
  }
  while (1) {
    left_m.run_pwm(PERIOD, PERIOD * turn * 0.7 / 0.65, DRIVE_MODE::BACKWARD);
    right_m.run_pwm(PERIOD, PERIOD * turn * 0.7, DRIVE_MODE::FORWARD);
    if (line_sensors[1] == 0) {
      left_m.run_pwm(PERIOD, 0, DRIVE_MODE::STOP);
      right_m.run_pwm(PERIOD, 0, DRIVE_MODE::STOP);
      usleep(300000);
      break;
    }
  }
  //書き込め！0番出るまで待て！
}

int turn_right() {
  left_m.run_pwm(PERIOD, PERIOD * turn, DRIVE_MODE::FORWARD);
  right_m.run_pwm(PERIOD, PERIOD * turn, DRIVE_MODE::BACKWARD);
  while (1) {
    if (line_sensors[3] == 0) {
      break;
    }
  }
  while (1) {
    left_m.run_pwm(PERIOD, PERIOD * turn * 0.7, DRIVE_MODE::FORWARD);
    right_m.run_pwm(PERIOD, PERIOD * turn * 0.7, DRIVE_MODE::BACKWARD);
    if (line_sensors[2] == 0) {
      left_m.run_pwm(PERIOD, 0, DRIVE_MODE::STOP);
      right_m.run_pwm(PERIOD, 0, DRIVE_MODE::STOP);
      usleep(300000);
      break;
    }
  }
}

int turn_control() {

  if (line_sensors[0] == 0 || line_sensors[3] == 0) {
    usleep(10000);
  } else {
    return 0;
  }

  if (line_sensors[0] == 0) {
    left_m.run_pwm(PERIOD, PERIOD*max*0.5, DRIVE_MODE::FORWARD);
    right_m.run_pwm(PERIOD, PERIOD*max*0.5, DRIVE_MODE::FORWARD);
    while (line_sensors[0] == 0)
      ;
//    left_m.run_pwm(PERIOD, PERIOD, DRIVE_MODE::STOP);
//    right_m.run_pwm(PERIOD, PERIOD, DRIVE_MODE::STOP);
//    usleep(500000);
    turn_left();
    flag = 'N';
  } else if (line_sensors[3] == 0) {
    left_m.run_pwm(PERIOD, PERIOD*max*0.5, DRIVE_MODE::FORWARD);
    right_m.run_pwm(PERIOD, PERIOD*max*0.5, DRIVE_MODE::FORWARD);

    while (line_sensors[3] == 0)
      ;
//    left_m.run_pwm(PERIOD, PERIOD, DRIVE_MODE::STOP);
//    right_m.run_pwm(PERIOD, PERIOD, DRIVE_MODE::STOP);
//    usleep(500000);
    turn_right();
    flag = 'N';
  }
}

int main() {

  //実行開始用スイッチ
  //GPIO::GPIO run_sw("P8_15");

  init_linetrace();
  init_supersonic();

  std::thread th_sonic(read_supersonic);
  std::thread th_line(read_line_th);
  left_m.run_pwm(PERIOD, PERIOD * 0.05 / 0.65, DRIVE_MODE::FORWARD);
  right_m.run_pwm(PERIOD, PERIOD * 0.05, DRIVE_MODE::FORWARD);
  sleep(1);  //超音波センサが０を返すので一秒待つ

  while (1) {

    //明示的にコピー可能なint型を渡してあげる必要がある。std::atomicはコピー不可のため
    printf("%d %d %d %d %d[mm]\r", line_sensors[0].load(),
           line_sensors[1].load(), line_sensors[2].load(),
           line_sensors[3].load(), distance_front.load());

    if (distance_front <= 50) {
      left_m.run_pwm(PERIOD, PERIOD * 0, DRIVE_MODE::STOP);
      right_m.run_pwm(PERIOD, PERIOD * 0, DRIVE_MODE::STOP);
      break;
    }

    waki.write(0);
    sleep(2);
    waki.write(180);
    sleep(2);

////    left_m.run_pwm(PERIOD, PERIOD * 0.2/0.65, DRIVE_MODE::BACKWARD);
////      right_m.run_pwm(PERIOD, PERIOD * 0.2, DRIVE_MODE::FORWARD);
//
//    if (line_sensors[1] == 0 && line_sensors[2] == 0) {
//      left_m.run_pwm(PERIOD, PERIOD * max, DRIVE_MODE::FORWARD);
//      right_m.run_pwm(PERIOD, PERIOD * max, DRIVE_MODE::FORWARD);
//    } else if (line_sensors[1] == 1) {
//      left_m.run_pwm(PERIOD, PERIOD * max, DRIVE_MODE::FORWARD);
//      right_m.run_pwm(PERIOD, PERIOD * low, DRIVE_MODE::FORWARD);
//
//    } else if (line_sensors[2] == 1) {
//      left_m.run_pwm(PERIOD, PERIOD * low, DRIVE_MODE::FORWARD);
//      right_m.run_pwm(PERIOD, PERIOD * max, DRIVE_MODE::FORWARD);
//    }
//
//    turn_control();

    if (utils::kbhit() == 'q') {
      break;
    }
  }
  return 0;
}
