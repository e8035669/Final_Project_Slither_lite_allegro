#include "Snake.h"

Snake* createSnake(Position position,char* name) {
	Snake* snake = malloc(sizeof(Snake));
	snake->length = 1;
	snake->isDead = 0;
	snake->headDirection = 0;
	snake->picSize = 0;

	Body* firstBody = malloc(sizeof(Body));
	firstBody->color = 1;
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
	Body* head = Body_getHead(snake);
	Body* tail = Body_getTail(snake);

	double degree = atan2(cursor.y,cursor.x);
	double tmp = degree-snake->headDirection;
	if(tmp>M_PI)tmp-=(M_PI*2.0);
	if(tmp<-M_PI)tmp+=(M_PI*2.0);
	const double threshold = 0.06;
	if(tmp > threshold){
        snake->headDirection += threshold;
	}else if(tmp < -threshold){
		snake->headDirection -=  threshold;
	}else{
		snake->headDirection = degree;
	}

	head->current_position.x += (cos(snake->headDirection)*speed);
	head->current_position.y += (sin(snake->headDirection)*speed);

	while(head!=tail){
		if(head->next){
			Position p1 = head->current_position;
			Position p2 = head->next->current_position;
			head->next->current_position.x += ((p1.x - p2.x)*0.3);
			head->next->current_position.y += ((p1.y - p2.y)*0.3);
			}
		head = head->next;
	}
/*
	while(tail!=head) {
		tail->current_position = tail->prev->current_position;
		tail = tail->prev;
	}
	if((!cursor.x)&&(!cursor.y))(cursor.y)++;
	double distance = sqrt(cursor.x*cursor.x+cursor.y*cursor.y);
	head->current_position.x += (cursor.x * speed/distance);
	head->current_position.y += (cursor.y * speed/distance);*/
}
