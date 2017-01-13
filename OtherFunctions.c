#include "OtherFunctions.h"

double distanceof(double x1,double y1,double x2,double y2) {
	return sqrt(abs(x1-x2)*abs(x1-x2)+abs(y1-y2)*abs(y1-y2));
}

Position p(double x,double y) {
	Position p = {x,y};
	return p;
}

Mouse m(int x,int y) {
	Mouse m = {x,y};
	return m;
}

int pow2(int x) {
	return x*x;
}

void eatenMusic(ALLEGRO_EVENT event,Position center,ALLEGRO_SAMPLE *eat) {
	long long x,y;
	long long distance2;
	int distanceMax2,xMax=1000,yMax=1000;
	double gain,pan;

	y=abs(event.user.data2-center.y);
	x=event.user.data1-center.x;

	distance2=x*x+y*y;
	distanceMax2=xMax*xMax+yMax*yMax;
	if(distance2>distanceMax2) return;
	else {
		gain=1-(distance2*1.0)/distanceMax2;
		pan=(double)x/xMax;
	//	if(x>=0) pan=pan+1;
	//	else  pan=pan-1;

		al_play_sample(eat,gain,pan,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
	}
}


