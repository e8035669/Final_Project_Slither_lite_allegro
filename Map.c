#include "Map.h"

Map* createMap(int mapSize,int lightSpot) {
	int i;
	Map* map = malloc(sizeof(Map));
	map->size = mapSize;
	map->totalLightSpotLength = 0;
	map->totalLightSpotSize = 0;
	map->containerSize = 100;
	lightSpotContainerInit(map);
	for(i=0; i<lightSpot; i++) {
		Put_LightSpot(map,Create_LightSpot(map->size));
	}
	return map;
}

void deleteMap(Map* map) {
	freeLightSpotContainer(map);
	free(map);
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


void outdeath(Snake* snakes[],int Ainumbers) {
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
void bodysdeath(Snake* snakes[],int Ainumbers,Map* map,int select) {
	int i;
	const int speed=11;
	const int picSize=snakes[0]->picSize;
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
			int k,difference;
			for(k=0; k<snakes[j]->length; k+=difference) {
				if(current_body->prev!=NULL) {
					long long distance2=0;
					distance2=(long long)((snakes[i]->head->current_position.x-current_x)*
										  (snakes[i]->head->current_position.x-current_x)+
										  (snakes[i]->head->current_position.y-current_y)*
										  (snakes[i]->head->current_position.y-current_y));
					if(abs(snakes[i]->head->current_position.x-current_x)<75) {
						if(abs(snakes[i]->head->current_position.y-current_y)<75) {
							if(distance2<picSize*picSize) {
								snakes[i]->isDead=2;
							}
						}
					}
					difference=((int)sqrt((double)distance2))/(speed*4);
					difference= (difference==0? 1:difference);
					int d;
					for(d=0; d<difference; d++) {
						if(current_body->prev!=NULL) {
							current_body=current_body->prev;
						} else {
							break;
						}
					}
					current_x=current_body->current_position.x;
					current_y=current_body->current_position.y;
				}
			}
		}
	}
}


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
