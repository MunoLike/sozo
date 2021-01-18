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

#define MOTOR_GAIN (0.95)
#define PERIOD (1000000)
Motor right_m("P9_14", "P9_12", "P8_26");
Motor left_m("P9_22", "P8_16", "P8_18", MOTOR_GAIN);
Servo waki("P8_13");

char flag = 'N';
float turn = 0.4;
float max = 0.5;
float low = 0.3;

float servo_cnt = 0;
bool servo_isup=true;

int turn_left() {
  left_m.run_pwm(PERIOD, PERIOD * turn / MOTOR_GAIN, DRIVE_MODE::BACKWARD);
  right_m.run_pwm(PERIOD, PERIOD * turn, DRIVE_MODE::FORWARD);

  while (1) {
    if (line_sensors[0] == 0) {
      break;
    }
  }
  while (1) {
    left_m.run_pwm(PERIOD, PERIOD * turn * 0.7 / MOTOR_GAIN,
                   DRIVE_MODE::BACKWARD);
    right_m.run_pwm(PERIOD, PERIOD * turn * 0.7, DRIVE_MODE::FORWARD);
    if (line_sensors[1] == 0) {
      left_m.run_pwm(PERIOD, 0, DRIVE_MODE::STOP);
      right_m.run_pwm(PERIOD, 0, DRIVE_MODE::STOP);
      usleep(300000);
      break;
    }
  }
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
    left_m.run_pwm(PERIOD, PERIOD * max * 0.5, DRIVE_MODE::FORWARD);
    right_m.run_pwm(PERIOD, PERIOD * max * 0.5, DRIVE_MODE::FORWARD);
    while (line_sensors[0] == 0)
      ;
    turn_left();
    flag = 'N';
  } else if (line_sensors[3] == 0) {
    left_m.run_pwm(PERIOD, PERIOD * max * 0.5, DRIVE_MODE::FORWARD);
    right_m.run_pwm(PERIOD, PERIOD * max * 0.5, DRIVE_MODE::FORWARD);

    while (line_sensors[3] == 0)
      ;
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
  left_m.run_pwm(PERIOD, PERIOD * 0.05, DRIVE_MODE::FORWARD);
  right_m.run_pwm(PERIOD, PERIOD * 0.05, DRIVE_MODE::FORWARD);
  sleep(1);
  while (1) {

    //明示的にコピー可能なint型を渡してあげる必要がある。std::atomicはコピー不可のため
//    printf("%d %d %d %d %d[mm]\r", line_sensors[0].load(),
//           line_sensors[1].load(), line_sensors[2].load(),
//           line_sensors[3].load(), distance_front.load());

//    if (distance_front <= 70) {
//      left_m.run_pwm(PERIOD, PERIOD * 0, DRIVE_MODE::STOP);
//      right_m.run_pwm(PERIOD, PERIOD * 0, DRIVE_MODE::STOP);
//      break;
//    }

//    left_m.run_pwm(PERIOD, PERIOD * max, DRIVE_MODE::FORWARD);
//      right_m.run_pwm(PERIOD, PERIOD * max, DRIVE_MODE::FORWARD);
//
    left_m.run_pwm(PERIOD, 0, DRIVE_MODE::FORWARD);
    right_m.run_pwm(PERIOD, 0, DRIVE_MODE::FORWARD);

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

//    servo
    if(servo_cnt > 180){
      servo_isup=false;
    }else if(servo_cnt<0){
      servo_isup=true;
    }

    if(servo_isup){
      servo_cnt+=1;
    }else{
      servo_cnt-=1;
    }

    waki.write(servo_cnt);

    if (utils::kbhit() == 'q') {
      left_m.run_pwm(PERIOD, PERIOD * 0, DRIVE_MODE::STOP);
      right_m.run_pwm(PERIOD, PERIOD * 0, DRIVE_MODE::STOP);
      break;
    }
  }
  return 0;
}
