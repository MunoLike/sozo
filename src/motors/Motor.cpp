/*
 * motor.cpp
 *
 *  Created on: 2020/11/05
 *      Author: MunoLike
 */
#include <fstream>
#include <fmt/core.h>

#include "../GPIO/GPIO.hpp"
#include "../GPIO/GPIOManager.h"
#include "../util/fileutil.hpp"
#include "Motor.hpp"

Motor::Motor(const std::string pwmpin, const std::string dir_cont1, const std::string dir_cont2)
    :
    Motor(pwmpin, dir_cont1, dir_cont2, 1.0) {
}

Motor::Motor(const std::string pwmpin, const std::string dir_cont1, const std::string dir_cont2, double gain)
    :
    pwm_pin_name(pwmpin),
    pwm_out(GPIO::GPIO(pwmpin)),
    dir_cont { GPIO::GPIO(dir_cont1), GPIO::GPIO(dir_cont2) },
    period(0),
    duty(0),
    gain(gain) {

  //OUTPUTに設定
  pwm_out.setDirection(GPIO::OUTPUT);
  pwm_out.setValue(GPIO::LOW);
  for (auto pin : dir_cont) {
    pin.setDirection(GPIO::OUTPUT);
    pin.setValue(GPIO::LOW);
  }

  /*PWM機能の有効化*/
  std::string mgr_file = utils::unclear_pathto_abs("/sys/devices/bone_capemgr.*/slots");

  std::ofstream cape_mgr(mgr_file);
  cape_mgr << "am33xx_pwm";
  cape_mgr.flush();  //dont forget

  /*ピンの設定（PIN_PWM指定のピン）*/
  std::string bone_pwm_path = "bone_pwm_" + pwmpin;

  cape_mgr << bone_pwm_path;
  cape_mgr.close();

  /*安全のため，PWM出力の停止*/

  std::string pwm_setting_dir = utils::unclear_pathto_abs(fmt::format("/sys/devices/ocp.*/pwm_test_{}.*", pwmpin));

  std::ofstream pwm_run(pwm_setting_dir + "run");
  pwm_run << 0;
  pwm_run.close();

  //  /*PWM周期の設定*/
  //  ここでは設定しない
  //  /*PWM極性の設定*/

  std::ofstream pwm_polarity(pwm_setting_dir + "polarity");
  pwm_polarity << 0;
  pwm_polarity.close();

  //  /*PWM　ON状態時間の初期化*/

  std::ofstream pwm_duty(pwm_setting_dir + "duty");
  pwm_duty << 0;
  pwm_duty.close();

  //  /*PWM出力の開始*/
  // ここではONにしない

}

void Motor::run_pwm(int period, int duty, DRIVE_MODE drive_mode) {
  this->duty = duty;
  this->period = period;

  std::string pwm_setting_dir = utils::unclear_pathto_abs(
      fmt::format("/sys/devices/ocp.*/pwm_test_{}.*/", this->pwm_pin_name));

  std::ofstream pwm_period(pwm_setting_dir + "period");
  pwm_period << period;
  pwm_period.close();

  std::ofstream pwm_duty(pwm_setting_dir + "duty");
  pwm_duty << ((int) (duty * gain));
  pwm_duty.close();

  std::ofstream pwm_run(pwm_setting_dir + "run");
  pwm_run << 1;
  pwm_run.close();

  this->dir_cont[0].setValue(static_cast<GPIO::PIN_VALUE>((int) drive_mode >> 0 & 0b1));
  this->dir_cont[1].setValue(static_cast<GPIO::PIN_VALUE>((int) drive_mode >> 1 & 0b1));
}

Motor::~Motor() {
  this->run_pwm(0, 0, DRIVE_MODE::STOP);

  std::string pwm_setting_dir = utils::unclear_pathto_abs(
      fmt::format("/sys/devices/ocp.*/pwm_test_{}.*/", this->pwm_pin_name));

  std::ofstream pwm_duty(pwm_setting_dir + "duty");
  pwm_duty << 0;
  pwm_duty.close();

  std::ofstream pwm_run(pwm_setting_dir + "run");
  pwm_run << 0;
  pwm_run.close();

  pwm_out.unexportPin();
  for (auto pin : dir_cont) {
    pin.setValue(GPIO::LOW);
    pin.unexportPin();
  }
}
