#include "supersonic.hpp"

#include <atomic>

#define gpio 27//P8_17

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

/* n 個のデータの単純選択ソートを行う */
void SimSelSort(int num[], int n) {
  int i, j, k, min, temp;

  for (i = 0; i < n - 1; i++) {
    min = num[i]; /* i 番目の要素を暫定的に最小値とし */
    k = i; /* 添字を保存 */
    for (j = i + 1; j < n; j++) {
      if (num[j] < min) { /* より小さい値が現れたら */
        min = num[j]; /* 最小値の入れ替え */
        k = j; /* 添字を保存 */
      }
    } /* このループを抜けるとk に最小値の添字が入っている */
    temp = num[i]; /* i 番目の要素と最小値の交換 */
    num[i] = num[k];
    num[k] = temp;
  }
  distance_front = num[3];
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
  int distance[7];
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
    for (int i = 0; i < 7; i++) {
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
      distance[i] = Ion / 61 * 10;
      usleep(50000);
    }
    SimSelSort(distance, 7);
  }
}

