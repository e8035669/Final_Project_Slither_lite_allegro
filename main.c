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
#include "ComputeTask.h"
#include "AiFunction.h"
#include "Animate.h"
#include "BodyStack.h"

int showMenu(char* retName);
int mainGameLoop(char* name,int Ainumbers,int lightspot,void (*aiExec)(Snake** snakes,Mouse* mouses,int Ainumbers));
int showLeaderBoard();
int main() {
	srand((unsigned int)time(NULL));
	init();
	initResources();
	int retCode = 0;
	char name[20] = "";
	retCode = showMenu(name);
	if(!retCode) {
		mainGameLoop(name,500,10000,AisBrain);
	}
	mainGameLoop(name,500,0,Aiattack);
	showLeaderBoard();

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
	char name[10] = "";

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

	/**< 殺蛇幾隻 一隻加1000分 */
    int killCount = 0;

	/**< 遊戲背景音樂 */
	al_play_sample(Res.bgMusic,1.0,ALLEGRO_AUDIO_PAN_NONE,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
	al_flush_event_queue(Res.eventQueue);


	while(!isDone) {
		ALLEGRO_EVENT event;
		al_wait_for_event(Res.eventQueue,&event);
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
							}
							Snake_beShorter(snake);
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
					outdeath(snakes,Ainumbers,&Res.eventSource);
					/**< 偵測蛇碰撞 */
					bodysdeath(snakes,Ainumbers,map,FPS%6,&Res.eventSource);


					/**< 死亡動畫 */
					for(i=0; i<Ainumbers; i++) {
						deathAnimate(snakes[i],map);
					}

					/**< FPS資料更新 */
					if(event.timer.count%60==0) {
						oldFPS = FPS;
						FPS = 0;
						mapUpdateLightSpotData(map);
					}

					if(oldFPS>59) {
						//*< 死掉會復活 */
						if(event.timer.count%6==0) {
							Snake_rebirth(snakes,Ainumbers);
						}
						//*< 有機率增加亮點 */
						if(rand()%100 < 10) {
							Put_LightSpot(map,Create_LightSpot(map->size));
						}
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
			case DEAD_EVENT:
				if(event.user.data1==0) {
					LeaderBoard_insertNewRecord(snake->name,event.user.data3+killCount*1000);
				}else{
                    if(event.user.data4==2&&event.user.data2==0){
						killCount++;
                    }
				}
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
		if(isDisplayNeedRefresh&&al_is_event_queue_empty(Res.eventQueue)) {
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

int showLeaderBoard() {
	int count = LeaderBoard_getRecordCount();
	Record* records = LeaderBoard_getRecordData(count);
	int retCode = 0;
	int isDone = 0;
	int needRedraw = 1;
	ALLEGRO_FONT* font = al_load_ttf_font("assets/ARCHRISTY.ttf",30,0);
	int screenH_2 = al_get_display_height(Res.display)>>1;
	int screenW_2 = al_get_display_width(Res.display)>>1;
	ALLEGRO_EVENT event;
	while(!isDone) {
		al_wait_for_event(Res.eventQueue,&event);
		switch(event.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				retCode = -1;
				isDone = 1;
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				if(event.keyboard.keycode==ALLEGRO_KEY_ENTER ||
						event.keyboard.keycode==ALLEGRO_KEY_PAD_ENTER||
						event.keyboard.keycode==ALLEGRO_KEY_ESCAPE)
					retCode = -1;
				isDone = 1;
				break;
			case ALLEGRO_EVENT_TIMER:
				if(needRedraw) {
					al_clear_to_color(al_map_rgb(0,0,0));
					al_draw_scaled_bitmap(Res.start,0,0,al_get_bitmap_width(Res.start),al_get_bitmap_height(Res.start)
										  ,0,0,al_get_display_width(Res.display),al_get_display_height(Res.display),0);

					int up = -150;
					al_draw_filled_rounded_rectangle(screenW_2-350,screenH_2+up-90,screenW_2+350,screenH_2+up+(count+1)*30,20,20,al_map_rgba(0,0,0,100));
					al_draw_text(font,al_map_rgb(255,255,255),screenW_2-320,screenH_2+up-50,0,"Name");
					al_draw_text(font,al_map_rgb(255,255,255),screenW_2,screenH_2+up-50,0,"Length");
					al_draw_text(font,al_map_rgb(255,255,255),screenW_2+150,screenH_2+up-50,0,"Play Time");
					int i;
					for(i=0; i<count; i++) {
						char tmp[30] = {0};
						strftime(tmp,30,"%m-%d %H:%M",localtime(&(records[i].timeStamp)));
						al_draw_textf(font,al_map_rgb(255,255,255),screenW_2-320,screenH_2+up+30*i,0,"%s",records[i].name);
						al_draw_textf(font,al_map_rgb(255,255,255),screenW_2,screenH_2+up+30*i,0,"%d",records[i].score);
						al_draw_textf(font,al_map_rgb(255,255,255),screenW_2+150,screenH_2+up+30*i,0,"%s",tmp);
					}
					al_flip_display();
				}
				break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				al_acknowledge_resize(Res.display);
				needRedraw = 1;
				screenH_2 = al_get_display_height(Res.display)>>1;
				screenW_2 = al_get_display_width(Res.display)>>1;
				break;

		}
	}
	return retCode;
}




