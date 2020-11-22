/*
 * GPIO.hpp
 *
 *  Created on: 2020/11/11
 *      Author: MunoLike
 */

#ifndef GPIO_GPIO_HPP_
#define GPIO_GPIO_HPP_
#include <string>

#include "GPIOManager.h"

namespace GPIO {

class GPIO {
 public:
  GPIO(const std::string key);
  virtual ~GPIO();

  int exportPin();
  int unexportPin();
  int setDirection(DIRECTION direction);
  int getDirection();
  int setValue(PIN_VALUE value);
  int getValue();
  int setEdge(EDGE_VALUE value);
  int getEdge();
  int waitForEdge(EDGE_VALUE value);

 private:
  unsigned int gpio_num;
};

}

#endif /* GPIO_GPIO_HPP_ */
