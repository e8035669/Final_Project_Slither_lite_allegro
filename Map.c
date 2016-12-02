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



LightSpot Create_LightSpot(void) {
	LightSpot lightspot;
	lightspot.x=rand()%(X_BORDER_MAX-X_BORDER_MIN)+X_BORDER_MIN;
	lightspot.y=rand()%(Y_BORDER_MAX-Y_BORDER_MIN)+Y_BORDER_MIN;
	lightspot.color=rand()%TOTAL_COLOR;
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


void Increase_LightSpotSize(Map *map,int size) {
	map->lightSpot=(LightSpot *) realloc(map->lightSpot,sizeof(LightSpot) *size);
	map->lightSpotSize=size;
}
