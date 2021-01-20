/*
 * waki_controller.hpp
 *
 *  Created on: 2021/01/20
 *      Author: MunoLike
 */

#ifndef CONTROLLER_WAKI_CONTROLLER_HPP_
#define CONTROLLER_WAKI_CONTROLLER_HPP_

#include "../motors/Servo.hpp"

namespace Control {
void init_waki(Servo *s);
void waki_move();
}

#endif /* CONTROLLER_WAKI_CONTROLLER_HPP_ */
