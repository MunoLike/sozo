/*
 * motor.cpp
 *
 *  Created on: 2020/11/05
 *      Author: MunoLike
 */

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <poll.h>
#include <termios.h>

#include "../gpio/gpio.hpp"
#include "Motor.hpp"

//各自の使用するボードで変更
/**************************/
const int OCP_NUM = 3;  //ocp.▲の▲に該当する番号
const int PWM_PERIOD = 10000000;
const int BONE_CAPEMGR_NUM = 9;  //bone_capemgr.●の●に該当
/**************************/

//各自の接続に応じて変更
/**************************/
char PIN_PWM[2][7] = { { "P9_14" }, { "P9_22" } };  //PWM有効化後の番号
int pwm_pin_num[2] = { 15, 16 };  //PWMに使用するのBBBピン番号
int motor_gpio_num[2][2] = { { 61, 60 }, { 65, 46 } };  //モータで使用するGPIO番号
/*************************/

//PWM初期化関数
void init_pwm(int motor_num) {
  int i, fd;
  char path[60], path3[60], path4[60];
  FILE *fp;
  for (i = 0; i < 2; i++) {
    gpio_export(motor_gpio_num[motor_num][i]);

    fd = gpio_open(motor_gpio_num[motor_num][i], "direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);

    sprintf(path3, "/sys/class/gpio/gpio%d/value",
            motor_gpio_num[motor_num][i]);
    fp = fopen(path3, "w");
    fprintf(fp, "%d", 0);
    fclose(fp);
  }

  /*PWM機能の有効化*/

  sprintf(path4, "/sys/devices/bone_capemgr.%d/slots", BONE_CAPEMGR_NUM);
  fp = fopen(path4, "w");
  fprintf(fp, "am33xx_pwm");
  fclose(fp);

  /*ピンの設定（PIN_PWM指定のピン）*/
  sprintf(path, "bone_pwm_%s", PIN_PWM[motor_num]);
  sprintf(path4, "/sys/devices/bone_capemgr.%d/slots", BONE_CAPEMGR_NUM);
  fp = fopen(path4, "w");
  fprintf(fp, path);
  fclose(fp);

  /*安全のため，PWM出力の停止*/
  sprintf(path, "/sys/devices/ocp.%d/pwm_test_%s.%d/run", OCP_NUM,
          PIN_PWM[motor_num], pwm_pin_num[motor_num]);
  fp = fopen(path, "wb");
  fprintf(fp, "%d", 0);
  fclose(fp);

  /*PWM周期の設定*/
  sprintf(path, "/sys/devices/ocp.%d/pwm_test_%s.%d/period", OCP_NUM,
          PIN_PWM[motor_num], pwm_pin_num[motor_num]);
  fp = fopen(path, "wb");
  fprintf(fp, "%d", PWM_PERIOD);
  fclose(fp);

  /*PWM極性の設定*/
  sprintf(path, "/sys/devices/ocp.%d/pwm_test_%s.%d/polarity", OCP_NUM,
          PIN_PWM[motor_num], pwm_pin_num[motor_num]);
  fp = fopen(path, "wb");
  fprintf(fp, "%d", 0);
  fclose(fp);

  /*PWM　ON状態時間の初期化*/
  sprintf(path, "/sys/devices/ocp.%d/pwm_test_%s.%d/duty", OCP_NUM,
          PIN_PWM[motor_num], pwm_pin_num[motor_num]);
  fp = fopen(path, "wb");
  fprintf(fp, "%d", 0);
  fclose(fp);

  /*PWM出力の開始*/
  sprintf(path, "/sys/devices/ocp.%d/pwm_test_%s.%d/run", OCP_NUM,
          PIN_PWM[motor_num], pwm_pin_num[motor_num]);
  fp = fopen(path, "wb");
  fprintf(fp, "%d", 1);
  fclose(fp);
}

//モータ用出力関数
void run_pwm(int motor_num, int duty, int drive_mode) {
  int i;
  char path[60], path3[60];
  FILE *fp;

  //一時停止
  if (drive_mode == 0) {
    for (i = 0; i < 2; i++) {
      sprintf(path3, "/sys/class/gpio/gpio%d/value",
              motor_gpio_num[motor_num][i]);
      fp = fopen(path3, "w");
      fprintf(fp, "%d", 1);
      fclose(fp);
    }
  }

  //モータ正転
  else if (drive_mode == 1) {
    for (i = 0; i < 2; i++) {
      sprintf(path3, "/sys/class/gpio/gpio%d/value",
              motor_gpio_num[motor_num][i]);
      fp = fopen(path3, "w");
      if (i == 0) {
        fprintf(fp, "%d", 1);
        fclose(fp);
      } else {
        fprintf(fp, "%d", 0);
        fclose(fp);
      }
    }

  }

  //モータ逆転
  else if (drive_mode == -1) {
    for (i = 0; i < 2; i++) {
      sprintf(path3, "/sys/class/gpio/gpio%d/value",
              motor_gpio_num[motor_num][i]);
      fp = fopen(path3, "w");
      if (i == 0) {
        fprintf(fp, "%d", 0);
        fclose(fp);
      } else {
        fprintf(fp, "%d", 1);
        fclose(fp);
      }
    }
  }

  //入力したdutyでPWM信号を出力
  sprintf(path, "/sys/devices/ocp.%d/pwm_test_%s.%d/duty", OCP_NUM,
          PIN_PWM[motor_num], pwm_pin_num[motor_num]);
  fp = fopen(path, "wb");
  fprintf(fp, "%d", duty);
  fclose(fp);
  usleep(200);
}

//PWM終了関数
void close_pwm(int motor_num) {
  FILE *fp;
  char path[60];
  int i;

  /*PWM　duty0出力*/
  sprintf(path, "/sys/devices/ocp.%d/pwm_test_%s.%d/duty", OCP_NUM,
          PIN_PWM[motor_num], pwm_pin_num[motor_num]);
  fp = fopen(path, "wb");
  fprintf(fp, "%d", 0);
  fclose(fp);

  /*PWM出力の停止*/
  sprintf(path, "/sys/devices/ocp.%d/pwm_test_%s.%d/run", OCP_NUM,
          PIN_PWM[motor_num], pwm_pin_num[motor_num]);
  fp = fopen(path, "wb");
  fprintf(fp, "%d", 0);
  fclose(fp);

  //GPIOの解放
  for (i = 0; i < 2; i++) {
    gpio_unexport(motor_gpio_num[motor_num][i]);
  }
}

Motor::Motor(const char *pwmpin, const char *dir_cont1, const char *dir_cont2,
             int pwm_period) {
  this->pwm_out = nullptr;

  this->dir_cont = { nullptr, nullptr };
  this->OCP_NUM = 0;
  this->BONE_CAPE_MGR_NUM = 0;
  this->PWM_PERIOD = pwm_period;
}

Motor::~Motor() {
}

void Motor::run_pwm(int duty, int drive_mode) {
}
