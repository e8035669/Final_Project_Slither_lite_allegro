#include "Map.h"

Map* createMap(int mapSize) {
	int i;
	Map* map = malloc(sizeof(Map));
	map->lightSpot = (LightSpot *)malloc(sizeof(LightSpot)*100);
	map->lightSpotSize = 100;
	map->size = mapSize;
	map->lightSpotLength = 100;
	for(i=0; i<100; i++) {
		map->lightSpot[i]=Create_LightSpot();
	}
	return map;
}

void deleteMap(Map* map) {
	free(map->lightSpot);
	free(map);
}

LightSpot Create_LightSpot() {
	return Create_LightSpot_xyc(rand()%(X_BORDER_MAX-X_BORDER_MIN)+X_BORDER_MIN,
								rand()%(Y_BORDER_MAX-Y_BORDER_MIN)+Y_BORDER_MIN,
								(rand()%TOTAL_COLOR)+1);
}

LightSpot Create_LightSpot_xy(int x,int y) {
	return Create_LightSpot_xyc(x,y,(rand()%TOTAL_COLOR)+1);
}

LightSpot Create_LightSpot_xyc(int x,int y,int color) {
	LightSpot lightspot;
	lightspot.x=x;
	lightspot.y=y;
	lightspot.color=color;
	return lightspot;
}

void Draw_LightSpot(Map *map,Snake *snake,ALLEGRO_BITMAP *lightspot) {
	int i;
	for(i=0; i<map->lightSpotSize; i++) {
		if((map->lightSpot[i].x > snake->head->current_position.x-SCREEN_W/2) &&
				(map->lightSpot[i].x < snake->head->current_position.x+SCREEN_W/2)&&
				(map->lightSpot[i].y > snake->head->current_position.y-SCREEN_H/2) &&
				(map->lightSpot[i].y < snake->head->current_position.y+SCREEN_H/2)
		  ) {
			al_draw_bitmap(lightspot,Pos(map->lightSpot[i].x,snake->head->current_position.x,0)-al_get_bitmap_width(lightspot)/2
						   ,Pos(map->lightSpot[i].y,snake->head->current_position.y,1)-al_get_bitmap_height(lightspot)/2,0);
		}
	}
}

void Increase_LightSpotSize(Map *map) {
	if(map->lightSpotSize - map->lightSpotLength <10) {
		LightSpot* tmp = realloc(map->lightSpot,sizeof(LightSpot)*(map->lightSpotSize)*2);
		if(tmp) {
			map->lightSpot = tmp;
			int i;
			LightSpot emptyLsp = Create_LightSpot_xyc(0,0,0);
			for(i=map->lightSpotSize; i < map->lightSpotSize*2 ; i++) {
				map->lightSpot[i] = emptyLsp;
			}
			map->lightSpotSize *= 2;
		}
	}
}

void Put_LightSpot(Map* map,LightSpot lSp) {
	Increase_LightSpotSize(map);
	int i;
	for(i=0; i<map->lightSpotSize; i++) {
		if(map->lightSpot[i].color == 0) {
			map->lightSpot[i] = lSp;
			map->lightSpotLength += 1;
			return;
		}
	}
}

void Eated_LightSpot(Map* map,int i) {
	if(map->lightSpot[i].color) {
		map->lightSpot[i] = Create_LightSpot_xyc(0,0,0);
		map->lightSpotLength--;
	}
}


