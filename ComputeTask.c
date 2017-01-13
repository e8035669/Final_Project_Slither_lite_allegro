#include "ComputeTask.h"
#include <math.h>
#include "Lightspot.h"
#include "BodyStack.h"

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
							degree=atan2(distY,distX);
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


void outdeath(Snake* snakes[],int Ainumbers,ALLEGRO_EVENT_SOURCE* eventSrc) {
	int i;
	for(i=0; i<Ainumbers; i++) {
		if(snakes[i]->isDead)continue;
		if(snakes[i]->head->current_position.x>9982||
				snakes[i]->head->current_position.x<18||
				snakes[i]->head->current_position.y>9982||
				snakes[i]->head->current_position.y<18) {
			snakes[i]->isDead=1;
			ALLEGRO_EVENT evt;
			evt.type = DEAD_EVENT;
			evt.user.data1=i;
			evt.user.data2 = -1;
			evt.user.data3=snakes[i]->length;
			evt.user.data4=1;
			al_emit_user_event(eventSrc,&evt,NULL);
		}
	}
}
void bodysdeath(Snake* snakes[],int Ainumbers,Map* map,int select,ALLEGRO_EVENT_SOURCE* eventSrc) {
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
		for(j=0; j<Ainumbers&& !snakes[i]->isDead; j++) {
			if(j==i || snakes[j]->isDead) continue;
			Body* current_body=snakes[j]->tail;
			double current_x=current_body->current_position.x;
			double current_y=current_body->current_position.y;
			int k,difference;
			for(k=0; k<snakes[j]->length&&!snakes[i]->isDead; k+=difference) {
				if(current_body->prev!=NULL) {
					long long distance2=0;
					distance2=(long long)((snakes[i]->head->current_position.x-current_x)*
										  (snakes[i]->head->current_position.x-current_x)+
										  (snakes[i]->head->current_position.y-current_y)*
										  (snakes[i]->head->current_position.y-current_y));
					if(abs(snakes[i]->head->current_position.x-current_x)<75) {
						if(abs(snakes[i]->head->current_position.y-current_y)<75) {
							if(distance2<picSize*picSize) {
								ALLEGRO_EVENT evt;
								evt.type = DEAD_EVENT;
								evt.user.data1=i;/**< 死掉的蛇 */
								evt.user.data2=j;/**< 被誰殺死的 */
								evt.user.data3=snakes[i]->length;/**< 死掉時的長度 */
								evt.user.data4=2;/**< 死亡原因 */
								al_emit_user_event(eventSrc,&evt,NULL);
								snakes[i]->isDead=2;
								break;
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
