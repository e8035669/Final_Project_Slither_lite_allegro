#include "AiFunction.h"
#include <math.h>
void AisBrain(Snake* snakes[],Mouse mouses[],int Ainumbers) {
	int i;
	for(i=1; i<Ainumbers; i++) {
		Body* current_body=snakes[0]->tail;
		double current_x=current_body->current_position.x;
		double current_y=current_body->current_position.y;

		if(snakes[i]->isDead) continue;
		int k;
		for(k=0; k<snakes[0]->length; k++) {
			if(current_body->prev!=NULL) {
				if(abs(snakes[i]->head->current_position.x-current_x)<300) {
					if(abs(snakes[i]->head->current_position.y-current_y)<300) {
						mouses[i].x=snakes[i]->head->current_position.x-current_x;
						mouses[i].y=snakes[i]->head->current_position.y-current_y;
					}
				}
				current_body=current_body->prev;
				current_x=current_body->current_position.x;
				current_y=current_body->current_position.y;
			}
		}
	}
}


void Aiattack(Snake* snakes[],Mouse mouses[],int Ainumbers) {
	int i;
	double current_x=snakes[0]->head->current_position.x+cos(snakes[0]->headDirection)*300;
	double current_y=snakes[0]->head->current_position.y+sin(snakes[0]->headDirection)*300;
	for(i=1; i<Ainumbers; i++) {
		if(snakes[i]->isDead) continue;
		if(abs(snakes[i]->head->current_position.x-current_x)<1000) {
			if(abs(snakes[i]->head->current_position.y-current_y)<1000) {
				mouses[i].x=current_x-snakes[i]->head->current_position.x;
				mouses[i].y=current_y-snakes[i]->head->current_position.y;
			}
		}
	}
}

void AiDefault(Snake* snakes[],Mouse mouses[],int Ainumbers){
}

