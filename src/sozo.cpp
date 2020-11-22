#include <thread>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "linetrace/linetrace.hpp"
#include "supersonic/supersonic.hpp"
#include "util/console_util.hpp"

int main() {
  init_linetrace();
  init_supersonic();

  //pthread_t handle;
  //pthread_create(&handle, NULL, read_supersonic, NULL);
  std::thread th(read_supersonic);

  while (1) {
    read_linetrace();
    printf("%d %d %d %d %d[mm]\r", line_sensors[0], line_sensors[1], line_sensors[2], line_sensors[3],
           distance_front.load());

    if (utils::kbhit() == 'q') {
      break;
    }
  }
  return 0;
}
