/*
 * Servo.hpp
 *
 *  Created on: 2020/12/10
 *      Author: MunoLike
 */

#ifndef MOTORS_SERVO_HPP_
#define MOTORS_SERVO_HPP_

#include "../GPIO/GPIO.hpp"

//Only Use for SG90
class Servo {

 public:
  Servo(const std::string pwmpin);
  virtual ~Servo();
  void write(int angle);

 private:
  std::string pwm_pin_name;
  GPIO::GPIO pwm_out;
  int angle;
};




#endif /* MOTORS_SERVO_HPP_ */
