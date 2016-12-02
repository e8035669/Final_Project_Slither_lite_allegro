#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

#include "Structures.h"
#include "kdebug/debug_c.h"
#include "define.h"
#include "draw_map.h"
#include "draw_snake.h"
#include "Map.h"
#include "Snake.h"
#include "OtherFunctions.h"
#include "kdebug/debug_c.h"


int main() {
	srand((unsigned int)time(NULL));
	al_set_app_name("Slither Lite by ....");
	al_init();
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_acodec_addon();
	al_install_audio();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	LOG("%-30s[OK]","Allegro initial");

	ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W,SCREEN_H);

	ALLEGRO_BITMAP *start,*start_button,*start_button_blink;
	start = al_load_bitmap( "assets/start.jpg");
	start_button = al_load_bitmap( "assets/start_button.png");
	start_button_blink = al_load_bitmap( "assets/start_button_blink.png");
	assert(start&&start_button&&start_button_blink);

	ALLEGRO_BITMAP ***bitmap,*snake_body,*snake_head,*lightspot;
	bitmap=(ALLEGRO_BITMAP ***) malloc(sizeof(ALLEGRO_BITMAP **) *PICTURE_NUM);
	int i;
	for(i=0; i<PICTURE_NUM; i++) {
		bitmap[i]=(ALLEGRO_BITMAP **) malloc(sizeof(ALLEGRO_BITMAP *) *PICTURE_NUM);
	}
	lightspot=al_load_bitmap("assets/ball.bmp");
	My_Allegro_Load_Map(bitmap);
	snake_body=al_load_bitmap("assets/snake/snake_body.png");
	snake_head=al_load_bitmap("assets/snake/snake_head.png");
	assert(lightspot&&snake_body&&snake_head);
	LOG("%-30s[OK]","Load Bitmaps");

	ALLEGRO_TIMER* timer = al_create_timer(1.0/60.0);
	ALLEGRO_FONT *pongFont = al_load_ttf_font("assets/ARCHRISTY.ttf", 70, 0);
	assert(pongFont);

	ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
	ALLEGRO_EVENT event;
	al_register_event_source(eventQueue,al_get_display_event_source(display));
	al_register_event_source(eventQueue,al_get_keyboard_event_source());
	al_register_event_source(eventQueue,al_get_mouse_event_source());
	al_register_event_source(eventQueue,al_get_timer_event_source(timer));
	LOG("%-30s[OK]","Allegro Resource");
	/**
	 *	此處初始化所有Allegro所需的元件
	 */

	char name[20] = {0};
	Snake* snake = createSnake(p(4000,4000),"default");
	Map* map = createMap(MAP_SIZE);
	al_start_timer(timer);
	const int displayHalfWidth = al_get_display_width(display)/2;
	const int displayHalfHeight = al_get_display_height(display)/2;
	int isFocusOnPlay = 0;
	int isDisplayRunning = 1;
	int isMouseBtnDown = 0;
	Mouse mouse = m(1,1);
	int isDisplayNeedRefresh = 1;
	LOG("%-30s[OK]","Map Creating");

	enum GameState {LoginDisplay,Gameing,Dead,LeaderBoard} gameState;
	gameState = LoginDisplay;
	LOG("%-30s[OK]","Game Start");
	while(isDisplayRunning) {
		al_wait_for_event(eventQueue,&event);

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
						if(event.mouse.x>500&&event.mouse.x<750&&event.mouse.y>400&&event.mouse.y<550) {
							isFocusOnPlay = 1;
							isDisplayNeedRefresh = 1;
						} else {
							isFocusOnPlay = 0;
							isDisplayNeedRefresh = 1;
						}
						break;
					case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
						if(event.mouse.x>500&&event.mouse.x<750&&event.mouse.y>400&&event.mouse.y<550) {
							strcpy(snake->name,name);
							gameState = Gameing;
						}
						break;
					case ALLEGRO_EVENT_DISPLAY_CLOSE:
						isDisplayRunning = 0;
						break;
					case ALLEGRO_EVENT_TIMER:
						if(isDisplayNeedRefresh&&al_is_event_queue_empty(eventQueue)) {
							isDisplayNeedRefresh = 0;
							al_clear_to_color(al_map_rgb(0,0,0));
							al_draw_scaled_bitmap(start,0,0,al_get_bitmap_width(start),al_get_bitmap_height(start)
												  ,0,0,al_get_display_width(display),al_get_display_height(display),0);
							if(isFocusOnPlay) {
								al_draw_bitmap( start_button_blink,SCREEN_W/2-150,SCREEN_H/2, 0);
							} else {
								al_draw_bitmap( start_button,SCREEN_W/2-150,SCREEN_H/2, 0);
							}
							al_draw_textf( pongFont, al_map_rgba(110, 239, 35,254), SCREEN_W/2, 100, ALLEGRO_ALIGN_CENTRE, "Enter your name:");
							al_draw_textf(pongFont,al_map_rgba(209,27,27,255),SCREEN_W/2,200,ALLEGRO_ALIGN_CENTRE,"%s",name);
							al_flip_display();
						}
				}
				break;
			case Gameing:
				switch(event.type) {
					case ALLEGRO_EVENT_MOUSE_AXES:
						mouse = m(event.mouse.x-displayHalfWidth,
								  event.mouse.y-displayHalfHeight);
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

						if(isDisplayNeedRefresh&&al_is_event_queue_empty(eventQueue)) {
							al_clear_to_color(al_map_rgb(0,0,0));
							Draw_Map(Body_getHead(snake)->current_position.x,Body_getHead(snake)->current_position.y,bitmap);
							Draw_LightSpot(map,snake,lightspot);
							Draw_Snake(snake,snake_body,snake_head);
							al_flip_display();
						}
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

	al_stop_timer(timer);
	al_destroy_timer(timer);
	al_destroy_event_queue(eventQueue);
	al_destroy_bitmap(start);
	al_destroy_bitmap(start_button);
	al_destroy_bitmap(start_button_blink);
	al_destroy_display(display);
	al_destroy_font(pongFont);
	My_Allegro_Destroy_Map(bitmap);
	return 0;
}
