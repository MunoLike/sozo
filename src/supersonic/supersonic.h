/*
 * supersonic.h
 *
 *  Created on: 2020/11/20
 *      Author: haruk
 */


#ifndef SUPERSONIC_SUPERSONIC_H_
#define SUPERSONIC_SUPERSONIC_H_

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

void init_supersonic();
void read_supersonic();


int distance_front;



#endif /* SUPERSONIC_SUPERSONIC_H_ */
