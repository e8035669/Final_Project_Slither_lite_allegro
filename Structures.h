#ifndef STRUCTURES_H
#define STRUCTURES_H

#include"kdebug/debug_c.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
typedef struct Position {
	double x;
	double y;
} Position;

typedef struct Mouse {
	int x;
	int y;
} Mouse;

typedef struct Body {
	struct Body* prev;

	int type;
	int color;
	Position current_position;
//	Position next_position;

	struct Body* next;
} Body;

typedef struct Snake {
	char name[20];
	int length;
	int picSize;
	Body* head;
	Body* tail;
	double headDirection;
	int isDead;
} Snake;

typedef struct LightSpot {
	int x;
	int y;
	int color;
} LightSpot;

typedef struct Map {
	LightSpot* lightSpot;
	int lightSpotLength;
	int lightSpotSize;
	int size;
} Map;



#endif // STRUCTURES_H
