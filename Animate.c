#include "Animate.h"
#include "Snake.h"
#include "Lightspot.h"
#include "BodyStack.h"
void deathAnimate(Snake* snake,Map* map) {
	if(snake->isDead==2) {
		if(snake->length>1) {
			int i = (snake->length)/10 + 1;
			while(i--) {
				Body* tmp = Body_getTail(snake);
				if(snake->length%3==0 && tmp) {
					Put_LightSpot(map,Create_LightSpot_xy(tmp->current_position.x+rand()%10-4,tmp->current_position.y+rand()%10-4));
				}
				Snake_beShorter(snake);
			}
		} else {
			snake->isDead = 3;
		}
	} else if(snake->isDead==1) {
		if(snake->length>1) {
			int i = (snake->length)/10 + 1;
			while(i--) {
				Snake_beShorter(snake);
			}
		} else {
			snake->isDead = 3;
		}
	}
}
