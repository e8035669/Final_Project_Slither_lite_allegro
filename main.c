#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <omp.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

#include "Init.h"
#include "Structures.h"
#include "kdebug/debug_c.h"
#include "define.h"
#include "draw_map.h"
#include "draw_snake.h"
#include "Map.h"
#include "Snake.h"
#include "OtherFunctions.h"
#include "LeaderBoard.h"

int showMenu(char* retName);
int mainGameLoop(char* name);
int main() {
	srand((unsigned int)time(NULL));
	init();
	initResources();
	int retCode = 0;
	char name[20] = "";
	retCode = showMenu(name);
	if(!retCode) {
		mainGameLoop(name);
	}
	/*死掉後要新增一筆遊玩紀錄
		並且顯示排行榜前十名*/


	destroy();
	return 0;
}



int showMenu(char* retName) {
	ALLEGRO_EVENT event;
	int isDone = 0;
	int retCode = 0;
	int isDisplayNeedRefresh = 1;
	int isFocusOnPlay = 0;
	char name[20] = "";
	al_flush_event_queue(Res.eventQueue);
	while(!isDone) {
		al_wait_for_event(Res.eventQueue,&event);
		switch(event.type) {
			case ALLEGRO_EVENT_KEY_CHAR:
				if(event.keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
					retCode = -1;
					isDone = 1;
				}
				if(event.keyboard.keycode==ALLEGRO_KEY_BACKSPACE) {
					if(strlen(name)>0) {
						name[strlen(name)-1] = '\0';
						isDisplayNeedRefresh = 1;
					}
				}
				if(event.keyboard.unichar>=32 && event.keyboard.unichar<=126) {
					if(strlen(name)<sizeof(name)-1) {
						name[strlen(name)] = event.keyboard.unichar;
						isDisplayNeedRefresh = 1;
					}
				}
				break;
			case ALLEGRO_EVENT_MOUSE_AXES:
				if(abs(event.mouse.x-(al_get_display_width(Res.display)>>1))<90 && abs(event.mouse.y-(al_get_display_height(Res.display)>>1)-120)<90) {
					isFocusOnPlay = 1;
					isDisplayNeedRefresh = 1;
				} else {
					isFocusOnPlay = 0;
					isDisplayNeedRefresh = 1;
				}
				break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				if(abs(event.mouse.x-al_get_display_width(Res.display)/2)<90 && abs(event.mouse.y-al_get_display_height(Res.display)/2-120)<90) {
					strcpy(retName,name);
					retCode = 0;
					isDone = 1;
				}
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				retCode = -1;
				isDone = 1;
				break;
			case ALLEGRO_EVENT_TIMER:
				if(isDisplayNeedRefresh&&al_is_event_queue_empty(Res.eventQueue)) {
					isDisplayNeedRefresh = 0;
					al_clear_to_color(al_map_rgb(0,0,0));
					al_draw_scaled_bitmap(Res.start,0,0,al_get_bitmap_width(Res.start),al_get_bitmap_height(Res.start)
										  ,0,0,al_get_display_width(Res.display),al_get_display_height(Res.display),0);
					if(isFocusOnPlay) {
						al_draw_bitmap(Res.start_button_blink,al_get_display_width(Res.display)/2-150,al_get_display_height(Res.display)/2, 0);
					} else {
						al_draw_bitmap(Res.start_button,al_get_display_width(Res.display)/2-150,al_get_display_height(Res.display)/2, 0);
					}
					al_draw_textf(Res.pongFont, al_map_rgba(110, 239, 35,254), al_get_display_width(Res.display)/2, 100, ALLEGRO_ALIGN_CENTRE, "Enter your name:");
					al_draw_textf(Res.pongFont,al_map_rgba(209,27,27,255),al_get_display_width(Res.display)/2,200,ALLEGRO_ALIGN_CENTRE,"%s",name);
					al_flip_display();
				}
				break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				al_acknowledge_resize(Res.display);
				isDisplayNeedRefresh = 1;
				break;
		}
	}
	return retCode;
}


int mainGameLoop(char* name) {
	Snake* snake = createSnake(p(5000,5000),name);
	Snake* snakes[Ainumbers];
	snakes[0]=snake;
	aiSnakes(snakes);
	Map* map = createMap(10000);
	Mouse mouse = m(1,1);
	Mouse mouses[Ainumbers]= {0};
	int i;
	for(i=1; i<Ainumbers; i++) {
		mouses[i].x=rand();
		mouses[i].y=rand();
	}
	int retCode = 0;
	int isDone = 0;
	int FPS = 0;
	int oldFPS = 0;
	int isMouseBtnDown = 0;
	int isDisplayNeedRefresh = 1;
	al_flush_event_queue(Res.eventQueue);
	while(!isDone) {
		ALLEGRO_EVENT event;
		al_wait_for_event(Res.eventQueue,&event);
		switch(event.type) {
			case ALLEGRO_EVENT_MOUSE_AXES: {
					int x = event.mouse.x-(al_get_display_width(Res.display)>>1);
					int y = event.mouse.y-(al_get_display_height(Res.display)>>1);
					if((x*x+y*y)>(snake->picSize*snake->picSize/4)) {
						mouse = m(x,y);
					}
				}
				break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				isMouseBtnDown = 1;
				break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
				isMouseBtnDown = 0;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				retCode = -1;
				isDone = 1;
				break;
			case ALLEGRO_EVENT_TIMER:
				if(isMouseBtnDown&&snake->length>10) {
					moveSnake(snake,mouse,10);
					Snake_beShorter(snake);
				} else {
					moveSnake(snake,mouse,5);
				}
				isDisplayNeedRefresh = 1;
				#pragma omp parallel for
				for(i=1; i<Ainumbers; i++) {
					moveSnake(snakes[i],mouses[i],10);
					if(event.timer.count%60==0) {
						//LOG("mouses[%d] %d %d",i,mouses[i].x,mouses[i].y);
						mouses[i].x=rand()-(RAND_MAX>>1);
						mouses[i].y=rand()-(RAND_MAX>>1);
					}
				}

				if(isDisplayNeedRefresh&&al_is_event_queue_empty(Res.eventQueue)) {
					FPS++;
					detectLightSpot(Body_getHead(snake),map,snake);
					#pragma omp parallel for
					for(i=1; i<Ainumbers; i++) { //讓AI蛇吃到亮點也變長
						detectLightSpot(Body_getHead(snakes[i]),map,snakes[i]);
					}
					al_clear_to_color(al_map_rgb(0,0,0));
					Draw_Map(snake->head->current_position.x,snake->head->current_position.y,Res.bitmap,Res.display,map);
					Draw_LightSpot(map,snake,Res.lightspot,Res.display);
					Draw_Snake(snake,Res.snake_body,Res.snake_head,Res.display,snake->head->current_position.x,snake->head->current_position.y);
					for(i=1; i<Ainumbers; i++) {
						Draw_Snake(snakes[i],Res.snake_body,Res.snake_head,Res.display,snake->head->current_position.x,snake->head->current_position.y);
					}
					al_draw_textf(Res.builtinFont,al_map_rgb(255,0,0),0,al_get_display_height(Res.display)-8,0,
								  "FPS:%d Pos(%.0f,%.0f) Length:%d Lsp:%d LspSize:%d",
								  oldFPS,snake->head->current_position.x,snake->head->current_position.y,snake->length,map->lightSpotLength,map->lightSpotSize);
					al_flip_display();
				}
				if(map->lightSpotLength<20000) {
					if(rand()%100 < 30) {
						Put_LightSpot(map,Create_LightSpot(map->size));
					}
				}

				if(event.timer.count%60==0) {
					//LOG("(%.2f,%.2f) angle %.2f lightspot length %d",
					//	Body_getHead(snake)->current_position.x,Body_getHead(snake)->current_position.y,snake->headDirection*180.0/3.1415926,map->lightSpotLength);
					oldFPS = FPS;
					FPS = 0;
				}
				break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				al_acknowledge_resize(Res.display);
				isDisplayNeedRefresh = 1;
				break;
		}
	}

	deleteSnake(snake);
	deleteMap(map);
	return retCode;
}






