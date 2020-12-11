/*
 * core.cpp
 *
 *  Created on: 2020/11/23
 *      Author: MunoLike
 */
#include <thread>

#include "core.hpp"
#include "com.hpp"

namespace Control {
std::thread *recv_th = NULL;

void setup() {
  if (recv_th == NULL) {
    recv_th = new std::thread(getCoords);
  }
}

void event_loop() {

}
}
