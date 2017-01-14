#include "Snake.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "OtherFunctions.h"
#include "BodyStack.h"

Snake* createSnake(Position position,char* name) {
	Snake* snake = malloc(sizeof(Snake));
	snake->length = 1;
	snake->isDead = 0;
	snake->headDirection = 0;
	snake->picSize = 0;

	Body* firstBody = malloc(sizeof(Body));
	firstBody->color = al_map_rgba(rand()%255,rand()%255,rand()%255,255-rand()%50);
	firstBody->current_position = position;
	firstBody->next = NULL;
	firstBody->prev = NULL;
	firstBody->type = 2;

	snake->head = firstBody;
	snake->tail = firstBody;
	int i;
	for(i=1; i<INIT_LENGTH; i++) {
		Snake_beLonger(snake);
	}
	strcpy(snake->name,name);
	assert(snake->length==INIT_LENGTH);
	return snake;
}

void deleteSnake(Snake* snake) {
	Body* body1 = Body_getHead(snake);
	Body* body2 = body1->next;
	while(body2) {
		free(body1);
		body1 = body2;
		body2 = body1->next;
	}
	free(body1);
	free(snake);
}

void moveSnake(Snake* snake,Mouse cursor,double speed) {
	if(!snake->isDead) {
		Body* head = Body_getHead(snake);
		Body* tail = Body_getTail(snake);

		double degree = atan2(cursor.y,cursor.x);
		double tmp = degree-snake->headDirection;
		if(tmp>M_PI)tmp-=(M_PI*2.0);
		if(tmp<-M_PI)tmp+=(M_PI*2.0);
		const double threshold = 0.06;
		if(tmp > threshold) {
			snake->headDirection += threshold;
		} else if(tmp < -threshold) {
			snake->headDirection -=  threshold;
		} else {
			snake->headDirection = degree;
		}

		head->current_position.x += (cos(snake->headDirection)*speed);
		head->current_position.y += (sin(snake->headDirection)*speed);

		while(head!=tail) {
			if(head->next) {
				Position p1 = head->current_position;
				Position p2 = head->next->current_position;
				head->next->current_position.x += ((p1.x - p2.x)*0.3);
				head->next->current_position.y += ((p1.y - p2.y)*0.3);
			}
			head = head->next;
		}
	}
}

void aiSnakes(Snake* snakes[],int Ainumbers) {
	Position pos1 = snakes[0]->head->current_position;
	int i;
	for(i=1; i<Ainumbers; i++) {
		char Ainame[5]= "";
		sprintf(Ainame,"%d",i);
		Position pos2 = p(rand()%10000,rand()%10000);
		while(abs(pos1.x-pos2.x)<300||abs(pos1.y-pos2.y)<300) {
			pos2.x = rand()%10000;
			pos2.y = rand()%10000;
		}
		snakes[i]=createSnake(pos2,Ainame);
	}
}


void Snake_rebirth(Snake* snakes[],int Ainumbers) {
	Position pos1 = snakes[0]->head->current_position;
	int i;
	for(i=1; i<Ainumbers; i++) {
		if(snakes[i]->isDead==3) {
			snakes[i]->isDead = 0;
			Position pos2 = p(rand()%10000,rand()%10000);
			while(abs(pos1.x-pos2.x)<300||abs(pos1.y-pos2.y)<300) {
				pos2.x = rand()%10000;
				pos2.y = rand()%10000;
			}
			snakes[i]->head->current_position = pos2;
			int j;
			for(j=snakes[i]->length; j<INIT_LENGTH; j++) {
				Snake_beLonger(snakes[i]);
			}
			break;
		}
	}
}



int snakeSpeedDelta(int speedMax,int speedMin,int accelerate,int slowDown,int speed,int speedDelta) {
	if(speed<=speedMin) {
		return accelerate;
	} else if(speed>=speedMax) {
		return slowDown;
	} else {
		return speedDelta;
	}
}
