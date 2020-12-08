/*
 * cliant.hpp
 *
 *  Created on: 2020/12/04
 *      Author: haruk
 */

#ifndef SOKET_CLIENT_HPP_
#define SOKET_CLIENT_HPP_

char buf[1024];
int client_sockfd;
void init_client();
int soket_client();
void finalize();

#endif /* SOKET_CLIENT_HPP_ */
