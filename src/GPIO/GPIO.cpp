/*
 * GPIO.cpp
 *
 *  Created on: 2020/11/11
 *      Author: MunoLike
 */
#include "GPIOManager.h"
#include "GPIOConst.h"
#include "GPIO.hpp"

namespace GPIO {


//key: give port Key
//ex) GPIO::GPIO led1("P9_12")
GPIO::GPIO(const char *key) {
  auto gpc = GPIOConst::getInstance();
  this->gpio_num = gpc->getGpioByKey(key);
  this->exportPin();
}

GPIO::~GPIO() {
}

int GPIO::exportPin() {
  GPIOManager *gp = GPIOManager::getInstance();
  return gp->exportPin(this->gpio_num);
}

int GPIO::unexportPin() {
  GPIOManager *gp = GPIOManager::getInstance();
  return gp->unexportPin(this->gpio_num);
}

int GPIO::setDirection(DIRECTION direction) {
  GPIOManager *gp = GPIOManager::getInstance();
  return gp->setDirection(this->gpio_num, direction);
}

int GPIO::getDirection() {
  GPIOManager *gp = GPIOManager::getInstance();
  return gp->getDirection(this->gpio_num);
}

int GPIO::setValue(PIN_VALUE value) {
  GPIOManager *gp = GPIOManager::getInstance();
  return gp->setValue(this->gpio_num, value);
}

int GPIO::getValue() {
  GPIOManager *gp = GPIOManager::getInstance();
  return gp->getValue(this->gpio_num);
}

int GPIO::setEdge(EDGE_VALUE value) {
  GPIOManager *gp = GPIOManager::getInstance();
  return gp->setEdge(this->gpio_num, value);
}

int GPIO::getEdge() {
  GPIOManager *gp = GPIOManager::getInstance();
  return gp->getEdge(this->gpio_num);
}

int GPIO::waitForEdge(EDGE_VALUE value) {
  GPIOManager *gp = GPIOManager::getInstance();
  return gp->waitForEdge(this->gpio_num, value);
}

}

