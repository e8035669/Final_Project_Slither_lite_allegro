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
#include "draw_lightspot.h"

int showMenu(char* retName);
int mainGameLoop(char* name,int Ainumbers,int lightspot,void (*aiExec)(Snake** snakes,Mouse* mouses,int Ainumbers));
int main() {
	srand((unsigned int)time(NULL));
	init();
	initResources();
	int retCode = 0;
	char name[20] = "";
	retCode = showMenu(name);
	if(!retCode) {
		mainGameLoop(name,20,10000,AisBrain);
	}
	/*死掉後要新增一筆遊玩紀錄
		並且顯示排行榜前十名*/


	destroy();
	return 0;
}


/** 遊戲開始畫面 */
int showMenu(char* retName) {
	ALLEGRO_EVENT event;
	int isDone = 0;
	int retCode = 0;
	int isDisplayNeedRefresh = 1;
	int isFocusOnPlay = 0;
	char name[20] = "";

	while(!isDone) {
		al_wait_for_event(Res.eventQueue,&event);
		switch(event.type) {
			case ALLEGRO_EVENT_KEY_CHAR:
				switch(event.keyboard.keycode) {
					case ALLEGRO_KEY_ESCAPE:
						retCode = -1;
						isDone = 1;
						break;
					case ALLEGRO_KEY_BACKSPACE:
						if(strlen(name)>0) {
							name[strlen(name)-1] = '\0';
							isDisplayNeedRefresh = 1;
						}
						break;
					case ALLEGRO_KEY_ENTER:
					case ALLEGRO_KEY_PAD_ENTER:
						strcpy(retName,name);
						retCode = 0;
						isDone = 1;
						break;
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


/**遊戲主體*/
int mainGameLoop(char* name,int Ainumbers,int lightspot,void (*aiExec)(Snake** snakes,Mouse* mouses,int Ainumbers)) {
	/**< 建立所有蛇 */
	Snake* snake = createSnake(p(5000,5000),name);
	Snake** snakes = calloc(sizeof(Snake*),Ainumbers);
	snakes[0]=snake;
	aiSnakes(snakes,Ainumbers);

	/**< 蛇前進的速度設定 */
	int speed=0,aiSpeed=0,speedDelta=1,aiSpeedDelta=1;
	/**< 切換視角設定 */
	int userControl=1;//value=1 while controlling yourself , 0 while looking other snakes
	char keyIn[5]= {0};
	int keyIn_int=0;
	/**< 建立地圖 */
	Map* map = createMap(10000,lightspot);
	/**< 建立所有滑鼠 */
	Mouse mouse = m(1,1);
	int isMouseBtnDown = 0;
	Mouse* mouses = calloc(sizeof(Mouse),Ainumbers);
	int i;
	for(i=1; i<Ainumbers; i++) {
		mouses[i].x=rand();
		mouses[i].y=rand();
	}
	/**< 遊戲結束控制 */
	int retCode = 0;
	int isDone = 0;
	/**< 畫面流暢度 */
	int FPS = 0;
	int oldFPS = 0;
	int isDisplayNeedRefresh = 1;

	/**< 遊戲背景音樂 */
	al_play_sample(Res.bgMusic,1.0,ALLEGRO_AUDIO_PAN_NONE,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
	al_flush_event_queue(Res.eventQueue);
	while(!isDone) {
		while(!al_is_event_queue_empty(Res.eventQueue)) {
			ALLEGRO_EVENT event;
			if(!al_get_next_event(Res.eventQueue,&event))break;
			switch(event.type) {
				/**< 更新滑鼠移動 */
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

				case ALLEGRO_EVENT_TIMER: {
						/**< 使用者的蛇 */
						if(userControl==1) {
							if(isMouseBtnDown&&snake->length>10) {
								al_set_sample_instance_playing(Res.slideSoundInstance,1);
								if(event.timer.count%15==7) {
									speedDelta=snakeSpeedDelta(9,6,2,-1,speed,speedDelta);
									speed+=speedDelta;
									Body* tmp = Body_getTail(snake);
									Put_LightSpot(map,Create_LightSpot_xy(tmp->current_position.x,tmp->current_position.y));
									int i;
									for(i=0; i<3; i++) {
										Snake_beShorter(snake);
									}
								}
							} else {
								al_set_sample_instance_playing(Res.slideSoundInstance,0);
								if(event.timer.count%10==5) {
									speedDelta=snakeSpeedDelta(5,3,1,-1,speed,speedDelta);
									speed+=speedDelta;
								}
							}
						}
						/**< 蛇移動 */
						if(userControl==1) {
							moveSnake(snake,mouse,speed);
						}
						/**< AI的蛇 */
						if(event.timer.count%15==3) {
							aiSpeedDelta=snakeSpeedDelta(5,3,1,-3,aiSpeed,aiSpeedDelta);
							aiSpeed+=aiSpeedDelta;
						}
						if(event.timer.count%60==0) {
							for(i=((userControl)?1:0); i<Ainumbers; i++) {
								mouses[i].x=rand()-(RAND_MAX>>1);
								mouses[i].y=rand()-(RAND_MAX>>1);
							}
							aiExec(snakes,mouses,Ainumbers);
						}
						/**< AI蛇移動 */
						#pragma omp parallel for
						for(i=((userControl)?1:0); i<Ainumbers; i++) {
							moveSnake(snakes[i],mouses[i],aiSpeed);
						}

						/**< 偵測吃掉附近亮點 */
						#pragma omp parallel for
						for(i=0; i<Ainumbers; i++) { //讓AI蛇吃到亮點也變長
							detectLightSpot(map,snakes[i],Res.eatSound,&Res.eventSource);
						}
						/**< 偵測界外 */
						outdeath(snakes,Ainumbers);
						/**< 偵測蛇碰撞 */
						bodysdeath(snakes,Ainumbers,map,FPS%6);


						/**< 死亡動畫 */
						for(i=0; i<Ainumbers; i++) {
							deathAnimate(snakes[i],map,event.timer.count);
						}
						/**< 有機率產生新的亮點 */
						if(rand()%100 < 10) {
							Put_LightSpot(map,Create_LightSpot(map->size));
						}
						/**< FPS資料更新 */
						if(event.timer.count%60==0) {
							oldFPS = FPS;
							FPS = 0;
							mapUpdateLightSpotData(map);
						}
						/**< 死掉的蛇復活 */
						if(event.timer.count%600==0) {
							Snake_rebirth(snakes,Ainumbers);
						}
						if(snake->isDead==3)isDone = 1;
						isDisplayNeedRefresh = 1;

					}
					break;
				case ALLEGRO_EVENT_DISPLAY_RESIZE:
					al_acknowledge_resize(Res.display);
					isDisplayNeedRefresh = 1;
					break;
				case EAT_EVENT_TYPE:
					eatenMusic(event,snakes[keyIn_int]->head->current_position,Res.eatSound);
					break;
				case ALLEGRO_EVENT_KEY_CHAR:
					/**< 數字輸入 */
					if(event.keyboard.keycode==ALLEGRO_KEY_SPACE) {
						if(userControl==1) userControl=0;
						else if(userControl==0 && keyIn_int==0) userControl=1;
					} else if(event.keyboard.keycode==ALLEGRO_KEY_PAD_ENTER) {
						keyIn_int = strtol(keyIn,NULL,10);
						if(keyIn_int>=Ainumbers) {
							LOG("not found snakes[%d]",keyIn_int);
							keyIn_int=0;
						}
						if(keyIn_int==0) userControl=1;
						else userControl=0;
						isDisplayNeedRefresh=1;
						memset(keyIn,0,sizeof(keyIn));
					} else if(event.keyboard.unichar>='0' && event.keyboard.unichar<='9') {
						int lenth=strlen(keyIn);
						if(lenth<sizeof(keyIn)-1) {
							keyIn[lenth]=event.keyboard.unichar;
						}
					}
					break;
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					retCode = -1;
					isDone = 1;
					break;
			}
		}


		if(isDisplayNeedRefresh) {
			FPS++;
			isDisplayNeedRefresh = 0;
			al_clear_to_color(al_map_rgb(0,0,0));
			Draw_Map(snakes[keyIn_int]->head->current_position,Res.bitmap,Res.display,map);
			Draw_LightSpot(map,snakes[keyIn_int],Res.lightspot,Res.display);
			Draw_Snake(snake,Res.snake_body,Res.snake_head,Res.display,snakes[keyIn_int]->head->current_position);
			for(i=1; i<Ainumbers; i++) {
				Draw_Snake(snakes[i],Res.snake_body,Res.snake_head,Res.display,snakes[keyIn_int]->head->current_position);
			}
			al_draw_textf(Res.builtinFont,al_map_rgb(255,0,0),0,al_get_display_height(Res.display)-8,0,
						  "FPS:%d Pos(%.0f,%.0f) Length:%d Lsp:%d LspSize:%d",
						  oldFPS,snake->head->current_position.x,snake->head->current_position.y,snake->length,map->totalLightSpotLength,map->totalLightSpotSize);
			al_draw_textf(Res.builtinFont,al_map_rgb(255,0,0),al_get_display_width(Res.display)-80,al_get_display_height(Res.display)-8,0,
						  "snakes:%d",keyIn_int);
			if(userControl) {
				al_draw_text(Res.builtinFont,al_map_rgb(255,0,0),0,0,0,"play mode");
			} else {
				al_draw_text(Res.builtinFont,al_map_rgb(255,0,0),0,0,0,"observe mode");
			}
			al_flip_display();
		}
	}

	deleteSnake(snake);
	deleteMap(map);
	return retCode;
}






