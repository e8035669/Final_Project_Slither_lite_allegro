#include "Map.h"

Map* createMap(int mapSize) {
	int i;
	Map* map = malloc(sizeof(Map));
	map->lightSpot = (LightSpot *)malloc(sizeof(LightSpot)*8192);
	map->lightSpotSize = 8192;
	map->size = mapSize;
	map->lightSpotLength = 7000;
	for(i=0; i<7000; i++) {
		map->lightSpot[i]=Create_LightSpot(mapSize);
	}
	return map;
}

void deleteMap(Map* map) {
	free(map->lightSpot);
	free(map);
}

LightSpot Create_LightSpot(int mapSize) {
	return Create_LightSpot_xyc(rand()%(mapSize),rand()%(mapSize),(rand()%TOTAL_COLOR)+1);
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


void Draw_LightSpot(Map *map,Snake *snake,ALLEGRO_BITMAP *lightspot,ALLEGRO_DISPLAY *display) {
	int i;

	for(i=0; i<map->lightSpotSize; i++) {
		if((map->lightSpot[i].x > snake->head->current_position.x-al_get_display_width(display)/2) &&
				(map->lightSpot[i].x < snake->head->current_position.x+al_get_display_width(display)/2) &&
				(map->lightSpot[i].y > snake->head->current_position.y-al_get_display_height(display)/2) &&
				(map->lightSpot[i].y < snake->head->current_position.y+al_get_display_height(display)/2)
		  ) {
			switch (map->lightSpot[i].color) {
				case 1:
					al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(255,0,0),0,0,50,50,Pos(map->lightSpot[i].x,snake->head->current_position.x,display,0),Pos(map->lightSpot[i].y,snake->head->current_position.y,display,1),15,15,0);
					break;
				case 2:
					al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(0,255,0),0,0,50,50,Pos(map->lightSpot[i].x,snake->head->current_position.x,display,0),Pos(map->lightSpot[i].y,snake->head->current_position.y,display,1),15,15,0);
					break;
				case 3:
					al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(0,0,255),0,0,50,50,Pos(map->lightSpot[i].x,snake->head->current_position.x,display,0),Pos(map->lightSpot[i].y,snake->head->current_position.y,display,1),15,15,0);
					break;
				case 4:
					al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(255,0,255),0,0,50,50,Pos(map->lightSpot[i].x,snake->head->current_position.x,display,0),Pos(map->lightSpot[i].y,snake->head->current_position.y,display,1),15,15,0);
					break;
				case 5:
					al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(0,255,255),0,0,50,50,Pos(map->lightSpot[i].x,snake->head->current_position.x,display,0),Pos(map->lightSpot[i].y,snake->head->current_position.y,display,1),15,15,0);
					break;
			}
		}
	}

}

void detectLightSpot(Body* Body_getHead, Map* map,Snake* snake) {
	int i=0;
	double degree=0,distance=0;

	for(i=0; i<map->lightSpotSize; i++) {
		if(map->lightSpot[i].color) {
			int distX = map->lightSpot[i].x-Body_getHead->current_position.x;
			int distY = map->lightSpot[i].y-Body_getHead->current_position.y;
			distance=sqrt(distX*distX+distY*distY);
			if(distance<snake->picSize+20) {
				degree=atan2(distY,distX);
				map->lightSpot[i].x-=cos(degree)*10;
				map->lightSpot[i].y-=sin(degree)*10;
				if(distance<snake->picSize) {
					int j=0,color = map->lightSpot[i].color;
					for(j=0; j<color; j++) {
						Snake_beLonger(snake);
					}
					Eated_LightSpot(map,i);
				}
			}
		}
	}
}

