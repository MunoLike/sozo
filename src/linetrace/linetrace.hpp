/*
 * linetrace.h
 *
 *  Created on: 2020/11/20
 *      Author: haruk
 */

#ifndef LINETRACE_LINETRACE_HPP_
#define LINETRACE_LINETRACE_HPP_

#include <array>
#include <atomic>

extern std::array<std::atomic<int>, 4> line_sensors;

void init_linetrace();
void read_linetrace();
void read_line_th();

#endif /* LINETRACE_LINETRACE_HPP_ */
