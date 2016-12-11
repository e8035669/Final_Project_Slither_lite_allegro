#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
//#include <omp.h>

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


int main() {
	srand((unsigned int)time(NULL));
	init();
	initResources();

	ALLEGRO_EVENT event;
	al_register_event_source(Res.eventQueue,al_get_display_event_source(Res.display));
	al_register_event_source(Res.eventQueue,al_get_keyboard_event_source());
	al_register_event_source(Res.eventQueue,al_get_mouse_event_source());
	al_register_event_source(Res.eventQueue,al_get_timer_event_source(Res.timer));

	char name[20] = {0};
	Snake* snake = createSnake(p(5000,5000),"default");
	Map* map = createMap(10000);

	int FPS = 0;
	int oldFPS = 0;
	int isFocusOnPlay = 0;
	int isDisplayRunning = 1;
	int isMouseBtnDown = 0;
	int isDisplayNeedRefresh = 1;
	Mouse mouse = m(1,1);
	LOG("%-30s[OK]","Map Creating");

	enum GameState {LoginDisplay,Gameing,Dead,LeaderBoard} gameState;
	gameState = LoginDisplay;
	LOG("%-30s[OK]","Game Start");
	al_start_timer(Res.timer);
	while(isDisplayRunning) {
		al_wait_for_event(Res.eventQueue,&event);

		switch(gameState) {
			/**
			*	此處顯示初始畫面Menu 需在畫面中接受使用者輸入名子
			*	名子限制20個字(暫定)
			*/
			case LoginDisplay:
				switch(event.type) {
					case ALLEGRO_EVENT_KEY_CHAR:
						if(event.keyboard.keycode==ALLEGRO_KEY_ESCAPE) {
							isDisplayRunning = 0;
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
							strcpy(snake->name,name);
							gameState = Gameing;
						}
						break;
					case ALLEGRO_EVENT_DISPLAY_CLOSE:
						isDisplayRunning = 0;
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
				break;
			case Gameing:
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
						isDisplayRunning = 0;
						break;
					case ALLEGRO_EVENT_TIMER:
						if(isMouseBtnDown) {
							moveSnake(snake,mouse,7);
						} else {
							moveSnake(snake,mouse,5);
						}
						isDisplayNeedRefresh = 1;

						if(isDisplayNeedRefresh&&al_is_event_queue_empty(Res.eventQueue)) {
							FPS++;
							detectLightSpot(Body_getHead(snake),map,snake);
							al_clear_to_color(al_map_rgb(0,0,0));
							Draw_Map(snake->head->current_position.x,snake->head->current_position.y,Res.bitmap,Res.display,map);
							Draw_LightSpot(map,snake,Res.lightspot,Res.display);
							Draw_Snake(snake,Res.snake_body,Res.snake_head,Res.display);
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
				break;
			case Dead:
				break;
			case LeaderBoard:
				break;
		}

		/*判斷有無光點可以吃*/
		/*如果有
			吃掉光點
			Snake_beLonger();變長*/
		/*判斷有無撞到其他蛇 or 邊界*/

	}
	/*死掉後要新增一筆遊玩紀錄
		並且顯示排行榜前十名*/

	deleteSnake(snake);
	deleteMap(map);
	destroy();
	return 0;
}
