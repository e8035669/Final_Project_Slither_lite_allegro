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
