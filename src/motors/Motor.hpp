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

class Motor {

public:
	Motor(const char *pwmpin, const char *dir_cont1, const char *dir_cont2, int pwm_period, int ocp_num, int bone_capemgr_num);
	virtual ~Motor();
	void run_pwm(int duty, int drive_mode);

private:
	GPIO::GPIO pwm_out;
	GPIO::GPIO dir_cont[2];
	int ocp_num;
	int pwm_period;
	int bone_capemgr_num;

};

#endif /* MOTORS_MOTOR_HPP_ */
