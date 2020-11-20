
#include <stdio.h>
#include <unistd.h>
#include "linetrace/linetrace.h"
#include "supersonic/supersonic.h"
#include "values.h"

int main() {
	init_linetrace();
	init_supersonic();

	while(1){
		read_linetrace();
		read_supersonic();
		printf("Distance=%d[mm]\n", distance_front);
		printf("%d %d %d %d\r",line_sensors[0],line_sensors[1],line_sensors[2],line_sensors[3]);
	}

	return 0;
}
