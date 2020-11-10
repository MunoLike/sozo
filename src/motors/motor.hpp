/*
 * motor.hpp
 *
 *  Created on: 2020/11/05
 *      Author: MunoLike
 */

#ifndef MOTORS_MOTOR_HPP_
#define MOTORS_MOTOR_HPP_


void init_pwm(int);
void run_pwm(int motor_num,int duty,int drive_mode);
void close_pwm(int motor_num);


class Motor{
public:
	int OCP_NUM;
	int PWM_PERIOD;
	int BONE_CAPE_MGR_NUM;

};

#endif /* MOTORS_MOTOR_HPP_ */
