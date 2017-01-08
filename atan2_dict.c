#include "atan2_dict.h"
#include <math.h>
double dict[1001][1001] = {0};
int inited = 0;

int atan2DictInit() {
	if(!inited) {
		int x = 0,y = 0;
		for(x=-500; x<=500; x++) {
			for(y=-500; y<=500; y++) {
				dict[y+500][x+500]=atan2(y,x);
			}
		}
		inited = 1;
		return 1;
	} else {
		return 0;
	}
}

double atan2_dict(int y,int x) {
	if(!inited)atan2DictInit();
	while(abs(y)>500) {
		y>>=1;
		x>>=1;
	}
	while(abs(x)>500) {
		y>>=1;
		x>>=1;
	}
	return dict[y+500][x+500];
}
