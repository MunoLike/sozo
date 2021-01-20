/*
 * waki_controller.cpp
 *
 *  Created on: 2021/01/20
 *      Author: MunoLike
 */

#include "../motors/Servo.hpp"
#include "waki_controller.hpp"

namespace Control {

Servo *servo;

void init_waki(Servo *s) {
  servo = s;
}

void waki_move() {
  float servo_cnt = 0;
  bool servo_isup = true;

  while (true) {
    //    servo
    if (servo_cnt > 180) {
      servo_isup = false;
    } else if (servo_cnt < 0) {
      servo_isup = true;
    }

    if (servo_isup) {
      servo_cnt += 0.5;
    } else {
      servo_cnt -= 0.5;
    }

    servo->write(servo_cnt);
  }
}
}
