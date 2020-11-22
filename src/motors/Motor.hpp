/*
 * motor.hpp
 *
 *  Created on: 2020/11/05
 *      Author: MunoLike
 */

#ifndef MOTORS_MOTOR_HPP_
#define MOTORS_MOTOR_HPP_

void init_pwm(int);
void run_pwm(int motor_num, int duty, int drive_mode);
void close_pwm(int motor_num);
//////////////////////////////////////////////////////////////
#include "../GPIO/GPIO.hpp"

enum class DRIVE_MODE : int {
  FORWARD = 0b10,  //前進
  BACKWARD = 0b01,  //後退
  STOP = 0b11,  //力を加えて止める
  FREE = 0b00,  //自由回転
};

class Motor {

 public:
  Motor(const std::string pwmpin, const std::string dir_cont1, const std::string dir_cont2);
  virtual ~Motor();
  void run_pwm(int period, int duty, DRIVE_MODE drive_mode);

 private:
  std::string pwmpin_name;
  GPIO::GPIO pwm_out;
  GPIO::GPIO dir_cont[2];
  int period, duty;
};

#endif /* MOTORS_MOTOR_HPP_ */
