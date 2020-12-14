/*
 * Servo.cpp
 *
 *  Created on: 2020/12/10
 *      Author: MunoLike
 */

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
#include "Servo.hpp"

Servo::Servo(const std::string pwmpin)
    :
    pwm_pin_name(pwmpin),
    pwm_out(GPIO::GPIO(pwmpin)),
    angle(0) {

  //OUTPUTに設定
  pwm_out.setDirection(GPIO::OUTPUT);
  pwm_out.setValue(GPIO::LOW);

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
  pwm_polarity << 1;
  pwm_polarity.close();

  //  /*PWM period初期化*/
  std::ofstream pwm_period(pwm_setting_dir + "period");
  pwm_period << 20000000;
  pwm_period.close();

  //  /*PWM　ON状態時間の初期化*/

  std::ofstream pwm_duty(pwm_setting_dir + "duty");
  pwm_duty << 0;
  pwm_duty.close();

  //  /*PWM出力の開始*/
  // ここではONにしない

}

void Servo::write(int angle_in) {
  this->angle = angle_in;
  int ns = (int) (((19.0 / 180) * (this->angle) + 5) * 100000);

  std::string pwm_setting_dir = utils::unclear_pathto_abs(
      fmt::format("/sys/devices/ocp.*/pwm_test_{}.*/", this->pwm_pin_name));

  std::ofstream pwm_duty(pwm_setting_dir + "duty");
  pwm_duty << ns;
  pwm_duty.close();

  std::ofstream pwm_run(pwm_setting_dir + "run");
  pwm_run << 1;
  pwm_run.close();

}

Servo::~Servo() {

}

