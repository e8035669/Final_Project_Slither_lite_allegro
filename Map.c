#include "Map.h"

Map* createMap(int mapSize) {
	int i;
	Map* map = malloc(sizeof(Map));
	map->size = mapSize;
	map->totalLightSpotLength = 0;
	map->totalLightSpotSize = 0;
	map->containerSize = 100;
	lightSpotContainerInit(map);
	for(i=0; i<10000; i++) {
		Put_LightSpot(map,Create_LightSpot(map->size));
	}
	return map;
}

void deleteMap(Map* map) {
	freeLightSpotContainer(map);
	free(map);
}

void lightSpotContainerInit(Map* map) {
	map->lightSpotContainers = calloc(sizeof(LightSpotContainer),map->containerSize*map->containerSize);
	int i,j;
	for(i=0; i<map->containerSize; i++) {
		for(j=0; j<map->containerSize; j++) {
			map->lightSpotContainers[i*map->containerSize+j].writeMutex = al_create_mutex();
			map->lightSpotContainers[i*map->containerSize+j].lightSpotSize = 10;
			map->lightSpotContainers[i*map->containerSize+j].lightSpotLength = 0;
			map->lightSpotContainers[i*map->containerSize+j].lightSpot = calloc(sizeof(LightSpot),10);
		}
	}
}
void freeLightSpotContainer(Map* map) {
	int i,j;
	for(i=0; i<map->containerSize; i++) {
		for(j=0; j<map->containerSize; j++) {
			free(map->lightSpotContainers[i*map->containerSize+j].lightSpot);
		}
	}
	free(map->lightSpotContainers);
	map->lightSpotContainers = NULL;
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


void Increase_LightSpotSize(LightSpotContainer* container) {
	if(container->lightSpotSize - container->lightSpotLength < 2) {
		LightSpot* tmp = realloc(container->lightSpot,sizeof(LightSpot)*(container->lightSpotSize)*2);
		if(tmp) {
			container->lightSpot = tmp;
			int i;
			LightSpot emptyLsp = Create_LightSpot_xyc(0,0,0);
			for(i=container->lightSpotSize; i<container->lightSpotSize * 2; i++) {
				container->lightSpot[i] = emptyLsp;
			}
			container->lightSpotSize *=2;
		}
	}
}

LightSpotContainer* getLightSpotContainer_xy(Map* map,int index_x,int index_y) {
	if(index_x>=0 && index_x<map->containerSize && index_y>=0 && index_y<map->containerSize) {
		return (map->lightSpotContainers)+(index_y*map->containerSize+index_x);
	} else {
		return NULL;
	}
}
LightSpotContainer* getLightSpotContainer_P(Map* map,LightSpotPos lSpP) {
	return getLightSpotContainer_xy(map,lSpP.index_x,lSpP.index_y);
}
int getContainerIndex(Map* map,int pos) {
	if(pos<0)pos = 0;
	if(pos>=map->size)pos = map->size-1;
	return pos/map->containerSize;
}

LightSpotPos getLightSpotPos(Map* map,int x,int y) {
	int index_x = getContainerIndex(map,x);
	int index_y = getContainerIndex(map,y);
	LightSpotPos lSpP = {index_x,index_y};
	return lSpP;
}

void Put_LightSpot(Map* map,LightSpot lSp) {
	LightSpotContainer* container = getLightSpotContainer_P(map,getLightSpotPos(map,lSp.x,lSp.y));
	if(container) {
		al_lock_mutex(container->writeMutex);
		Increase_LightSpotSize(container);
		int i;
		for(i=0; i<container->lightSpotSize; i++) {
			if(container->lightSpot[i].color == 0) {
				container->lightSpot[i] = lSp;
				container->lightSpotLength += 1;
				break;
			}
		}
		al_unlock_mutex(container->writeMutex);
	}

}

void Eated_LightSpot(LightSpotContainer* container,int i) {
	if(container)if(container->lightSpot[i].color) {
			al_lock_mutex(container->writeMutex);
			container->lightSpot[i] = Create_LightSpot_xyc(0,0,0);
			container->lightSpotLength--;
			al_unlock_mutex(container->writeMutex);
		}
}


void Draw_LightSpot(Map *map,Snake *snake,ALLEGRO_BITMAP *lightspot,ALLEGRO_DISPLAY *display) {
	int i,j;
	int displayHalf_W=al_get_display_width(display)>>1;
	int displayHalf_H=al_get_display_height(display)>>1;

	Position p = snake->head->current_position;
	LightSpotPos pos1 = getLightSpotPos(map,p.x-displayHalf_W,p.y-displayHalf_H);
	LightSpotPos pos2 = getLightSpotPos(map,p.x+displayHalf_W,p.y+displayHalf_H);


	for(i=pos1.index_x; i<=pos2.index_x; i++) {
		for(j=pos1.index_y; j<=pos2.index_y; j++) {
			LightSpotContainer* container = getLightSpotContainer_xy(map,i,j);
			if(container) {
				int k;
				for(k=0; k<container->lightSpotSize; k++) {
					switch (container->lightSpot[k].color) {
						case 1:
							al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(255,0,0),0,0,50,50,container->lightSpot[k].x-p.x+displayHalf_W,container->lightSpot[k].y-p.y+displayHalf_H,15,15,0);
							break;
						case 2:
							al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(0,255,0),0,0,50,50,container->lightSpot[k].x-p.x+displayHalf_W,container->lightSpot[k].y-p.y+displayHalf_H,15,15,0);
							break;
						case 3:
							al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(0,0,255),0,0,50,50,container->lightSpot[k].x-p.x+displayHalf_W,container->lightSpot[k].y-p.y+displayHalf_H,15,15,0);
							break;
						case 4:
							al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(255,0,255),0,0,50,50,container->lightSpot[k].x-p.x+displayHalf_W,container->lightSpot[k].y-p.y+displayHalf_H,15,15,0);
							break;
						case 5:
							al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(0,255,255),0,0,50,50,container->lightSpot[k].x-p.x+displayHalf_W,container->lightSpot[k].y-p.y+displayHalf_H,15,15,0);
							break;
					}
				}
			}
		}
	}
}

void detectLightSpot(Map* map,Snake* snake,ALLEGRO_SAMPLE *eat,ALLEGRO_EVENT_SOURCE *eventSource) {
	if(snake->isDead)return;
	double degree=0,distance=0;
	Body* head = Body_getHead(snake);
	Position pos = head->current_position;
	LightSpotPos containerPos = getLightSpotPos(map,pos.x,pos.y);
	int i,j,k;
	for(i=containerPos.index_x-1; i<=containerPos.index_x+1; i++) {
		for(j=containerPos.index_y-1; j<=containerPos.index_y+1; j++) {
			LightSpotContainer* container = getLightSpotContainer_xy(map,i,j);
			if(container) {
				for(k=0; k<container->lightSpotSize; k++) {
					if(container->lightSpot[k].color) {
						int distX = container->lightSpot[k].x-pos.x;
						int distY = container->lightSpot[k].y-pos.y;
						distance=sqrt(distX*distX+distY*distY);
						if(distance<snake->picSize+20) {
							degree=atan2_dict(distY,distX);
							container->lightSpot[k].x-=cos(degree)*10;
							container->lightSpot[k].y-=sin(degree)*10;
							if(distance<snake->picSize) {
								int l=0,color = container->lightSpot[k].color;
								for(l=0; l<color; l++) {
									Snake_beLonger(snake);
								}
								ALLEGRO_EVENT event;
								event.user.type=EAT_EVENT_TYPE;
								event.user.data1=container->lightSpot[k].x;
								event.user.data2=container->lightSpot[k].y;
								al_emit_user_event(eventSource,&event,NULL);
								Eated_LightSpot(container,k);
							}
						}
					}
				}
			}
		}
	}
}


void outdeath(Snake* snakes[]) {
	int i;
	for(i=0; i<Ainumbers; i++) {
		if(snakes[i]->isDead)continue;
		if(snakes[i]->head->current_position.x>9982||
				snakes[i]->head->current_position.x<18||
				snakes[i]->head->current_position.y>9982||
				snakes[i]->head->current_position.y<18) {
			snakes[i]->isDead=1;
			//LOG("%d is out",i);
//			while(snakes[i]->length>1) {
//				Snake_beShorter(snakes[i]);
//			}
		}
	}
}
void bodysdeath(Snake* snakes[],Map* map,int select) {
	int i;
	int picSize=snakes[0]->picSize;
	int num = Ainumbers/6+1;
	int start = num*select;
	int end = (num*(select+1)>Ainumbers)?Ainumbers:num*(select+1);
	#pragma omp parallel for
	for(i=start; i<end; i++) {
		if(snakes[i]->isDead)continue;
		int j;
		for(j=0; j<Ainumbers; j++) {
			if(j==i || snakes[j]->isDead) continue;
			Body* current_body=snakes[j]->tail;
			double current_x=current_body->current_position.x;
			double current_y=current_body->current_position.y;
			int k;
			for(k=0; k<snakes[j]->length; k++) {
				if(current_body->prev!=NULL) {
					if(abs(snakes[i]->head->current_position.x-current_x)<75) {
						if(abs(snakes[i]->head->current_position.y-current_y)<75) {
							if(
								(snakes[i]->head->current_position.x-current_x)*
								(snakes[i]->head->current_position.x-current_x)+
								(snakes[i]->head->current_position.y-current_y)*
								(snakes[i]->head->current_position.y-current_y)<
								picSize*picSize) {
								snakes[i]->isDead=2;
							}
						}
					}
					current_body=current_body->prev;
					current_x=current_body->current_position.x;
					current_y=current_body->current_position.y;
				}
			}
		}
	}
}


void deathAnimate(Snake* snake,Map* map,int timerCount) {
	if(snake->isDead==2) {
		if(snake->length>1) {
			Body* tmp = Body_getTail(snake);
			if(timerCount%3==0 && tmp) {
				Put_LightSpot(map,Create_LightSpot_xy(tmp->current_position.x+rand()%10-4,tmp->current_position.y+rand()%10-4));
			}
			Snake_beShorter(snake);
		} else {
			snake->isDead = 3;
		}
	} else if(snake->isDead==1) {
		if(snake->length>1) {
			Snake_beShorter(snake);
		} else {
			snake->isDead = 3;
		}
	}
}


void AisBrain(Snake* snakes[],Mouse mouses[]) {
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

void mapUpdateLightSpotData(Map* map) {
	int totalLightSpotSize = 0;
	int totalLightSpotLength = 0;
	int i,j;
	for(i=0; i<map->containerSize; i++) {
		for(j=0; j<map->containerSize; j++) {
			totalLightSpotSize += (map->lightSpotContainers[i*map->containerSize+j].lightSpotSize);
			totalLightSpotLength += (map->lightSpotContainers[i*map->containerSize+j].lightSpotLength);
		}
	}
	map->totalLightSpotLength = totalLightSpotLength;
	map->totalLightSpotSize = totalLightSpotSize;
}
