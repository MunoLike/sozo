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
Motor right_m("P9_14", "P9_12", "P8_26");
Motor left_m("P9_22", "P8_16", "P8_18", 0.65);

int turn_left() {
  left_m.run_pwm(PERIOD, PERIOD * 0.2, DRIVE_MODE::BACKWARD);
  right_m.run_pwm(PERIOD, PERIOD * 0.2, DRIVE_MODE::FORWARD);

  while (1) {

    if (line_sensors[0] == 0) {
      break;
    }
  }
  while (1) {

    if (line_sensors[1] == 0) {
      break;
    }
  }
  while (1) {

    if (line_sensors[0] == 1) {
      break;
    }
  }
  //書き込め！0番出るまで待て！
}

int turn_right() {
  left_m.run_pwm(PERIOD, PERIOD * 0.2, DRIVE_MODE::FORWARD);
  right_m.run_pwm(PERIOD, PERIOD * 0.2, DRIVE_MODE::BACKWARD);

  while (1) {

    if (line_sensors[3] == 0) {
      break;
    }
  }
  while (1) {

    if (line_sensors[2] == 0) {
      break;
    }
  }
  while (1) {

    if (line_sensors[3] == 1) {
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
  //GPIO::GPIO run_sw("P8_15");
  char flag = 'N';
  init_linetrace();
  init_supersonic();

  std::thread th(read_supersonic);
  sleep(1);  //超音波センサが０を返すので一秒待つ
  while (1) {
    read_linetrace();
    printf("%d %d %d %d %d[mm]\r", line_sensors[0], line_sensors[1], line_sensors[2], line_sensors[3],
           distance_front.load());

    if (distance_front.load() <= 200) {
      left_m.run_pwm(PERIOD, PERIOD * 0, DRIVE_MODE::STOP);
      right_m.run_pwm(PERIOD, PERIOD * 0, DRIVE_MODE::STOP);
      break;
    }

//    left_m.run_pwm(PERIOD, PERIOD * 0.4, DRIVE_MODE::FORWARD);
//    right_m.run_pwm(PERIOD, PERIOD * 0, DRIVE_MODE::FORWARD);

    float max = 0.3;
    float low = 0.2;

    if (line_sensors[1] == 0 && line_sensors[2] == 0) {
      left_m.run_pwm(PERIOD, PERIOD * max, DRIVE_MODE::FORWARD);
      right_m.run_pwm(PERIOD, PERIOD * max, DRIVE_MODE::FORWARD);
      flag = 'N';
    } else if (line_sensors[1] == 1 || flag == 'R') {
      left_m.run_pwm(PERIOD, PERIOD * max, DRIVE_MODE::FORWARD);
      right_m.run_pwm(PERIOD, PERIOD * low, DRIVE_MODE::FORWARD);
      flag = 'R';
    } else if (line_sensors[2] == 1 || flag == 'L') {
      left_m.run_pwm(PERIOD, PERIOD * low, DRIVE_MODE::FORWARD);
      right_m.run_pwm(PERIOD, PERIOD * max, DRIVE_MODE::FORWARD);
      flag = 'L';
    }

    //turn_control();

    if (utils::kbhit() == 'q') {
      break;
    }
  }
  return 0;
}
