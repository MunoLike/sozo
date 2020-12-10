/*
 * core.hpp
 *
 *  Created on: 2020/12/10
 *      Author: User
 */

#ifndef CONTROLLER_CORE_HPP_
#define CONTROLLER_CORE_HPP_

#include <atomic>
#include <array>

namespace Control{
extern std::array<std::atomic<double>,4> RobotCoords;

void setup();
}


#endif /* CONTROLLER_CORE_HPP_ */
