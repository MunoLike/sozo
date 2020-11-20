#include <stdio.h>
#include <unistd.h>
#include "../variable.hpp"
#include "linetrace.hpp"

#define gpio0 60
#define gpio1 3
#define gpio2 49
#define gpio3 115


void init_linetrace(){
	//前準備
	FILE *fp,*fp0,*fp1,*fp2,*fp3;
	int in1=0;
	char path0[100],path1[100],path2[100],path3[100];

	printf("ライントレーサGPIO番号： %d %d %d %d\n", gpio0,gpio1,gpio2,gpio3);

	///////////////////////////////////////////////////////////////////////

		//gpioの有効化
	fp =fopen("/sys/class/gpio/export","w");
	fprintf(fp, "%d", gpio0);
	fclose(fp);

	//gpioを出力い設定
	sprintf(path0, "/sys/class/gpio/gpio%d/direction",gpio0);
	fp = fopen(path0,"w");
	fprintf(fp, "in");
	fclose(fp);

	///////////////////////////////////////////////////////////////////////

	fp =fopen("/sys/class/gpio/export","w");
	fprintf(fp, "%d", gpio1);
	fclose(fp);

	sprintf(path1, "/sys/class/gpio/gpio%d/direction",gpio1);
	fp = fopen(path1,"w");
	fprintf(fp, "in");
	fclose(fp);

	///////////////////////////////////////////////////////////////////////

	fp =fopen("/sys/class/gpio/export","w");
	fprintf(fp, "%d", gpio2);
	fclose(fp);

	sprintf(path2, "/sys/class/gpio/gpio%d/direction",gpio2);
	fp = fopen(path2,"w");
	fprintf(fp, "in");
	fclose(fp);

	////////////////////////////////////////////////////////////////////////

	fp =fopen("/sys/class/gpio/export","w");
	fprintf(fp, "%d", gpio3);
	fclose(fp);

	sprintf(path3, "/sys/class/gpio/gpio%d/direction",gpio3);
	fp = fopen(path3,"w");
	fprintf(fp, "in");
	fclose(fp);

}

void read_linetrace(){
	FILE *fp0,*fp1,*fp2,*fp3;
	char path0[100],path1[100],path2[100],path3[100];
	//gpio出力用valueファイル名をpathに格納
	sprintf(path0, "/sys/class/gpio/gpio%d/value",gpio0);
	sprintf(path1, "/sys/class/gpio/gpio%d/value",gpio1);
	sprintf(path2, "/sys/class/gpio/gpio%d/value",gpio2);
	sprintf(path3, "/sys/class/gpio/gpio%d/value",gpio3);

	fp0 = fopen(path0, "r");
	fp1 = fopen(path1, "r");
	fp2 = fopen(path2, "r");
	fp3 = fopen(path3, "r");
	//valueファイルを開き，出力値を書き込む

	fscanf(fp0, "%d",&line_sensors[0]);
	fscanf(fp1, "%d",&line_sensors[1]);
	fscanf(fp2, "%d",&line_sensors[2]);
	fscanf(fp3, "%d",&line_sensors[3]);



	fclose(fp0);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}

void finalize_linetrace(){
	FILE *fp;
	char path0[100],path1[100],path2[100],path3[100];
	//gpio出力用valueファイル名をpathに格納
	sprintf(path0, "/sys/class/gpio/gpio%d/value",gpio0);
	sprintf(path1, "/sys/class/gpio/gpio%d/value",gpio1);
	sprintf(path2, "/sys/class/gpio/gpio%d/value",gpio2);
	sprintf(path3, "/sys/class/gpio/gpio%d/value",gpio3);
	//valueファイルに，出力値0を書き込みGPIOをLOWにする．
	fp = fopen(path0, "w");
	fprintf(fp, "%d",0);
	fclose(fp);
	fp = fopen(path1, "w");
	fprintf(fp, "%d",0);
	fclose(fp);
	fp = fopen(path2, "w");
	fprintf(fp, "%d",0);
	fclose(fp);
	fp = fopen(path3, "w");
	fprintf(fp, "%d",0);
	fclose(fp);

	printf("ライントレーサプログラム終了\n");
}
