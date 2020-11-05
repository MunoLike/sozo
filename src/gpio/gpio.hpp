/*
 * gpio.hpp
 *
 *  Created on: 2020/11/05
 *      Author: MunoLike
 */

#ifndef GPIO_GPIO_HPP_
#define GPIO_GPIO_HPP_

void gpio_export(int n);
void gpio_unexport(int n);
int gpio_open(int n, char *file, int flag);




#endif /* GPIO_GPIO_HPP_ */
