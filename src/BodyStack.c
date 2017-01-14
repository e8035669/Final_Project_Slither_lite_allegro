#include "BodyStack.h"

#include <math.h>
Body* Body_getHead(Snake* snake) {
	return snake->head;
}

Body* Body_getTail(Snake* snake) {
	return snake->tail;
}

Body* Body_getBody(Snake* snake,int which) {
	int i=0;
	Body* thisBody = Body_getHead(snake);
	for(; i<which; i++) {
		if(thisBody->next) {
			thisBody = thisBody->next;
		} else {
			return NULL;
		}
	}
	return thisBody;
}

int Body_push(Snake* snake,Body* body) {
	assert(snake && snake->tail && body);
	body->prev = snake->tail;
	body->next = NULL;
	snake->tail->next = body;
	snake->tail = body;
	return 1;
}

Body* Body_pop(Snake* snake) {
	assert(snake && snake->tail);
	Body* tmp = snake->tail;
	if(tmp) {
		if(snake->tail->prev) {
			snake->tail = snake->tail->prev;
			snake->tail->next = NULL;
		} else {
			snake->head = NULL;
			snake->tail = NULL;
		}
		tmp->prev = NULL;
		return tmp;
	} else {
		return NULL;
	}
}

int Snake_beLonger(Snake* snake) {
	Body* newBody = malloc(sizeof(Body));
	Body* tail = Body_getTail(snake);
	newBody->type = tail->type;
	newBody->color = tail->color;
	newBody->current_position = tail->current_position;
//	newBody->next_position = tail->current_position;

	Body_push(snake,newBody);
	(snake->length)++;
	snake->picSize = Picture_Size(snake->length);
	return 1;
}

int Snake_beShorter(Snake* snake) {
//	if(snake->length<=INIT_LENGTH)return 0;
	Body* tmp = Body_pop(snake);
	if(tmp) {
		(snake->length)--;
		free(tmp);
		snake->picSize = Picture_Size(snake->length);
		return 1;
	} else {
		return 0;
	}
}

void Snake_lengthCheck(Snake* snake) {
	int count = 1;
	Body* thisBody = Body_getHead(snake);
	Body* tail = Body_getTail(snake);
	while(thisBody!=tail) {
		if(count > snake->length+10)break;
		thisBody = thisBody->next;
		count++;
	}
	assert(count == snake->length);
}


int Picture_Size(int length) {
	return (int)(log10(length)*10.0)+25;
}
