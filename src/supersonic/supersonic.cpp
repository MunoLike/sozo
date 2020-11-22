#include <atomic>

#include "supersonic.hpp"

#define gpio 27

std::atomic<int> distance_front;

//gpioの有効化関数
void gpio_export(int n) {
  int fd;
  char buf[40];
  sprintf(buf, "%d", n);
  fd = open("/sys/class/gpio/export", O_WRONLY);
  write(fd, buf, strlen(buf));
  close(fd);
}
//gpioの有効化解除の関数
void gpio_unexport(int n) {
  int fd;
  char buf[40];
  sprintf(buf, "%d", n);
  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  write(fd, buf, strlen(buf));
  close(fd);
}
//gpioの設定ファイルを開く関数
int gpio_open(int n, char *file, int flag) {
  int fd;
  char buf[40];
  sprintf(buf, "/sys/class/gpio/gpio%d/%s", n, file);
  fd = open(buf, flag);
  return fd;
}

//パルスのON時間を時刻から算出する関数 単位:micro sec
//timespec構造体の構造はtv_secに秒,tv_nsecに秒未満の時間をnano秒で表す

#define RC_LAP(n, o) ((n.tv_sec - o.tv_sec) * (1000*1000) + (n.tv_nsec - o.tv_nsec) / 1000)

void init_supersonic() {
  int fd;

  //gpioの有効化
  gpio_export(gpio);
  //gpioを入力に設定

  fd = gpio_open(gpio, "direction", O_WRONLY);
  write(fd, "%d", 2);
  close(fd);
  //gpioをedgeに設定
  fd = gpio_open(gpio, "edge", O_WRONLY);
  write(fd, "both", 4);
  close(fd);

}

void read_supersonic() {

  int fd;
  float Ion;
  int ret;
  char c;
  struct timespec origin;  // signal start time
  struct timespec now;  // signal change time
  struct pollfd pfd;
  //gpioのvalueファイルを開く
  fd = gpio_open(gpio, "value", O_RDONLY);
  pfd.fd = fd;  //監視するファイルを設定
  pfd.events = POLLPRI;  //監視する通知を設定
  while (1) {
    lseek(fd, 0, SEEK_SET);  //読み取り位置を先頭に設定
    ret = poll(&pfd, 1, -1);  //通知を監視
    read(fd, &c, 1);  //通知状態を読み込む
    if (c == '1') {  //パルスの立ち上がりの時刻を取得
      clock_gettime(CLOCK_MONOTONIC_RAW, &origin);
    } else
      continue;
    lseek(fd, 0, SEEK_SET);
    ret = poll(&pfd, 1, -1);
    read(fd, &c, 1);
    if (c == '0') {  //パルスの立ち下がりの時刻を取得
      clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    } else
      continue;
    Ion = RC_LAP(now, origin);  //パルスのON時間を算出
    distance_front = Ion / 61 * 10;
    usleep(50000);
  }
}

