/*
 * com.cpp
 *
 *  Created on: 2020/12/10
 *      Author: MunoLike
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#include "core.hpp"
#include "../util/strutil.hpp"

#define HOST_ADDR "192.168.11.59"

namespace Control{

std::array<std::atomic<double>,4> RobotCoords;

void getCoords() {
  int sockfd;
  struct sockaddr_in addr;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(50007);
  addr.sin_addr.s_addr = inet_addr(HOST_ADDR);

  connect(sockfd, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));

  char receive_str[100];
  while(true){
    if (send(sockfd, "giveme", 7, 0) < 0) {
      perror("send");
    } else {
      recv(sockfd, receive_str, 100, 0);
      std::string re(receive_str);
      auto coords = utils::splitDouble(re, ',');

      //TODO:lock
      Control::RobotCoords[0] = coords[0];
      Control::RobotCoords[1] = coords[1];
      Control::RobotCoords[2] = coords[2];
      Control::RobotCoords[3] = coords[3];

      printf("receive:%s\n", receive_str);
    }

    usleep(50000);
  }

  //never called
  close(sockfd);
}
}

