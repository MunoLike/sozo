/*
 * supersonic.h
 *
 *  Created on: 2020/11/20
 *      Author: haruk
 */

#ifndef SUPERSONIC_SUPERSONIC_HPP_
#define SUPERSONIC_SUPERSONIC_HPP_

#include <atomic>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <poll.h>
#include <time.h>

extern std::atomic<int> distance_front;

void init_supersonic();
void read_supersonic();

#endif /* SUPERSONIC_SUPERSONIC_HPP_ */
