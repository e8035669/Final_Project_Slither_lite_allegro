#include "Init.h"
#include <allegro5/allegro_native_dialog.h>
#include <windows.h>
int showInitScreen(char* itemName);
void punishHandle();
void init() {
	al_set_app_name("\u8caa\u5403\u86c7\u7684\u5371\u6a5f");
	al_init();
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_acodec_addon();
	al_install_audio();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_native_dialog_addon();
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	LOG("%-30s[OK]","Allegro initial");
}

void initResources() {
	//al_set_new_display_refresh_rate(60);
	al_set_new_display_option(ALLEGRO_VSYNC,1,ALLEGRO_REQUIRE);
	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	Res.eventQueue = al_create_event_queue();
	Res.display = al_create_display(800,600);
	Res.pongFont = al_load_ttf_font("assets/ARCHRISTY.ttf", 70, 0);
	if(!Res.pongFont)goto ERR;
	Res.builtinFont = al_create_builtin_font();
	al_set_target_backbuffer(Res.display);
	al_register_event_source(Res.eventQueue,al_get_display_event_source(Res.display));

	Res.icon=al_load_bitmap("assets/icon.png");
	if(showInitScreen("assets/icon.png")&& !Res.icon)goto ERR;
	al_set_display_icon(Res.display,Res.icon);

	Res.pianpianFont = al_load_ttf_font("assets/pianpian.ttf",70,0);
	if(showInitScreen("assets/pianpian.ttf")&&!Res.pianpianFont)goto ERR;

	Res.start = al_load_bitmap( "assets/start.jpg");
	if(showInitScreen("assets/start.jpg")&&!Res.start )goto ERR;
	Res.start_button = al_load_bitmap( "assets/buttons/start_button.png");
	if(showInitScreen("assets/buttons/start_button.png")&& !Res.start_button)goto ERR;
	Res.start_button_blink = al_load_bitmap( "assets/buttons/start_button_blink.png");
	if(showInitScreen("assets/buttons/start_button_blink.png")&& !Res.start_button_blink)goto ERR;
	Res.replayButton = al_load_bitmap("assets/buttons/replayButton.png");
	if(showInitScreen("assets/buttons/replayButton.png")&& !Res.replayButton)goto ERR;
	Res.copyrightButton = al_load_bitmap("assets/buttons/copyrightButton.png");
	if(showInitScreen("assets/buttons/copyrightButton.png")&& !Res.copyrightButton)goto ERR;
	Res.copyrightButton_blink = al_load_bitmap("assets/buttons/copyrightButton_blink.png");
	if(showInitScreen("assets/buttons/copyrightButton_blink.png")&& !Res.copyrightButton_blink)goto ERR;
	Res.leaderBoardButton = al_load_bitmap("assets/buttons/leaderBoardButton.png");
	if(showInitScreen("assets/buttons/leaderBoardButton.png")&& !Res.leaderBoardButton)goto ERR;
	Res.leaderBoardButton_blink = al_load_bitmap("assets/buttons/leaderBoardButton_blink.png");
	if(showInitScreen("assets/buttons/leaderBoardButton_blink.png")&& !Res.leaderBoardButton_blink)goto ERR;


	Res.bitmap = (ALLEGRO_BITMAP***)calloc(sizeof(ALLEGRO_BITMAP*),PICTURE_NUM);
	int i,j;
	for(i=0; i<PICTURE_NUM; i++) {
		Res.bitmap[i]=(ALLEGRO_BITMAP **) calloc(sizeof(ALLEGRO_BITMAP *),PICTURE_NUM);
	}
	char str[]="assets/bg/  .jpg";
	for(i=0; i<PICTURE_NUM; i++) {
		for(j=0; j<PICTURE_NUM; j++) {
			str[10]=i+'0';
			str[11]=j+'0';
			Res.bitmap[i][j]=al_load_bitmap(str);
			if(showInitScreen(str)&& !Res.bitmap[i][j] )goto ERR;
		}
	}

	Res.snake_body = al_load_bitmap("assets/snake/snake_body.png");
	if(showInitScreen("assets/snake/snake_body.png") && !Res.snake_body)goto ERR;
	Res.snake_head = al_load_bitmap("assets/snake/snake_head.png");
	if(showInitScreen("assets/snake/snake_head.png")&& !Res.snake_head)goto ERR;
	Res.lightspot = al_load_bitmap("assets/lightspot/lightspot.png");
	if(showInitScreen("assets/lightspot/lightspot.png")&& !Res.lightspot )goto ERR;
	Res.timer = al_create_timer(1.0/60.0);

	al_register_event_source(Res.eventQueue,al_get_keyboard_event_source());
	al_register_event_source(Res.eventQueue,al_get_mouse_event_source());
	al_register_event_source(Res.eventQueue,al_get_timer_event_source(Res.timer));
	al_start_timer(Res.timer);

	al_init_user_event_source(&Res.eventSource);
	al_register_event_source(Res.eventQueue,&Res.eventSource);

	if(!al_reserve_samples(100)) LOG("reserve sample fail");
	else {
		Res.bgMusic=al_load_sample("assets/music/bgm.wav");
		if(!Res.bgMusic && showInitScreen("assets/music/bgm.wav"))goto ERR;
		Res.crashSound=al_load_sample("assets/music/crash.wav");
		if(!Res.crashSound && showInitScreen("assets/music/crash.wav"))goto ERR;
		Res.eatSound=al_load_sample("assets/music/eat.wav");
		if(!Res.eatSound && showInitScreen("assets/music/eat.wav"))goto ERR;
		Res.slideSound=al_load_sample("assets/music/slide.wav");
		if(!Res.slideSound && showInitScreen("assets/music/slide.wav"))goto ERR;

		Res.defaultMixer=al_get_default_mixer();
		Res.slideSoundInstance=al_create_sample_instance(Res.slideSound);
		al_set_sample_instance_speed(Res.slideSoundInstance,1.0);
		al_set_sample_instance_gain(Res.slideSoundInstance,1.0);
		al_set_sample_instance_pan(Res.slideSoundInstance,0.0);
		al_set_sample_instance_playmode(Res.slideSoundInstance,ALLEGRO_PLAYMODE_ONCE);
		if(!al_attach_sample_instance_to_mixer(Res.slideSoundInstance,Res.defaultMixer)) LOG("attach sample instance to mixer fail");
	}
	LOG("%-30s[OK]","Allegro Resource Loading");
	/**< 遊戲背景音樂 */
	al_play_sample(Res.bgMusic,1.0,ALLEGRO_AUDIO_PAN_NONE,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
	return;
ERR:
	LOG("Loading File ERROR!!");
	punishHandle();
	exit(0);
}

void destroy() {

	al_destroy_display(Res.display);
	al_destroy_font(Res.pongFont);
	al_destroy_font(Res.builtinFont);
	al_destroy_bitmap(Res.start);
	al_destroy_bitmap(Res.start_button);
	al_destroy_bitmap(Res.start_button_blink);
	al_destroy_bitmap(Res.icon);
	al_destroy_bitmap(Res.replayButton);
	al_destroy_bitmap(Res.copyrightButton);
	al_destroy_bitmap(Res.copyrightButton_blink);
	al_destroy_bitmap(Res.leaderBoardButton);
	al_destroy_bitmap(Res.leaderBoardButton_blink);
	int i,j;
	for(i=0; i<PICTURE_NUM; i++) {
		for(j=0; j<PICTURE_NUM; j++) {
			al_destroy_bitmap(Res.bitmap[i][j]);
		}
	}
	for(i=0; i<PICTURE_NUM; i++) {
		free(Res.bitmap[i]);
	}
	free(Res.bitmap);
	al_destroy_bitmap(Res.snake_body);
	al_destroy_bitmap(Res.snake_head);
	al_destroy_bitmap(Res.lightspot);
	al_stop_timer(Res.timer);
	al_destroy_timer(Res.timer);
	al_destroy_user_event_source(&Res.eventSource);
	al_destroy_event_queue(Res.eventQueue);

	al_destroy_sample(Res.bgMusic);
	al_destroy_sample(Res.eatSound);
	al_destroy_sample(Res.crashSound);
	al_detach_sample_instance(Res.slideSoundInstance);
	al_destroy_sample_instance(Res.slideSoundInstance);
	al_destroy_sample(Res.slideSound);
	//al_uninstall_audio();
}

int showInitScreen(char* itemName) {
	al_clear_to_color(al_map_rgb(96,96,96));
	al_draw_text(Res.pongFont,al_map_rgb(220,220,220),al_get_display_width(Res.display)>>1,
				 (al_get_display_height(Res.display)>>1)-100,ALLEGRO_ALIGN_CENTRE,"Loading...");
	al_draw_text(Res.builtinFont,al_map_rgb(220,220,220),al_get_display_width(Res.display)>>1,
				 (al_get_display_height(Res.display)>>1)+30,ALLEGRO_ALIGN_CENTER,itemName);
	al_flip_display();//Show loading screen
	return 1;
}


#define E(title,header,msg) al_show_native_message_box(Res.display,title,header,msg,NULL,ALLEGRO_MESSAGEBOX_ERROR);
#define Q(title,header,msg) al_show_native_message_box(Res.display,title,header,msg,NULL,ALLEGRO_MESSAGEBOX_WARN|ALLEGRO_MESSAGEBOX_YES_NO);
void punishHandle() {
	int ret = 0;
	E("\u4f60\u5b8c\u86cb\u4e86","\u6a94\u6848\u8b80\u53d6\u6642\u767c\u751f\u932f\u8aa4","\u6216\u8a31\u9700\u8981\u91cd\u65b0\u5b89\u88dd");
	/**< 你完蛋了 檔案讀取時發生錯誤 或許需要重新安裝 */

	E("\u6211\u5011\u7684\u7a0b\u5f0f\u600e\u9ebc\u639b\u4e86","\u4f60\u505a\u4e86\u4ec0\u9ebc!!","\u70ba\u4ec0\u9ebc\u6211\u5011\u7684\u7a0b\u5f0f\u58de\u4e86");
	/**< E("我們的程式怎麼掛了","你做了什麼!!","為什麼我們的程式壞了"); */
	E("\u6211\u5011\u5f88\u96e3\u904e","\u6211\u5011\u4e09\u500b\u4eba\u5beb\u4e86\u90a3\u9ebc\u4e45\u7684\u7a0b\u5f0f","\u73fe\u5728\u537b\u4e00\u884c\u4e5f\u4e0d\u80fd\u57f7\u884c");
	/**< E("我們很難過","我們三個人寫了那麼久的程式","現在卻一行也不能執行"); */
	ret = Q("\u70ba\u4ec0\u9ebc!!!!","\u70ba\u4ec0\u9ebc\u6703\u9019\u6a23","\u662f\u6211\u5011\u7684\u5b89\u88dd\u6a94\u6709bug\u55ce\uff1f");
	/**< ret = Q("為什麼!!!!","為什麼會這樣","是我們的安裝檔有bug嗎？"); */
	if(ret){
		ret = Q("\u771f\u7684\u55ce\uff1f","\u771f\u7684\u55ce\uff1f","\u771f\u7684\u55ce\uff1f\u771f\u7684\u55ce\uff1f\u771f\u7684\u55ce\uff1f\u771f\u7684\u55ce\uff1f\u771f\u7684\u55ce\uff1f");
		/**< Q("真的嗎？","真的嗎？","真的嗎？真的嗎？真的嗎？真的嗎？真的嗎？"); */
		if(ret){
			E("\u6211\u5011\u6703\u76e1\u5feb\u4fee\u5fa9\u597dbug","\u597d...\u90a3...\u8acb\u901a\u77e5\u6211","\u8acb\u64a5\u62530978178333\u5c07\u7531\u5c08\u4eba\u70ba\u60a8\u670d\u52d9");
			/**< E("我們會盡快修復好bug","好...那...請通知我","請撥打0978178333將由專人為您服務"); */
			E("\u62ac\u69d3","\u90a3\u73fe\u5728...","");
			/**< E("抬槓","那現在...",""); */
			E("\u62ac\u69d3","\u53cd\u6b63\u6211\u7684\u7a0b\u5f0f\u4e5f\u58de\u5566","");
			/**< E("抬槓","反正我的程式也壞啦",""); */
			E("\u62ac\u69d3","\u4e0d\u5982\u6211\u5011\u4f86\u804a\u5929\u597d\u4e86","");
			/**< E("抬槓","不如我們來聊天好了",""); */
			E("\u62ac\u69d3","\u4f60\u8aaa\u904a\u6232\u55ce?","\u904a\u6232\u6539\u5929\u518d\u73a9\u5566~");
			/**< E("抬槓","你說遊戲嗎?","遊戲改天再玩啦~"); */
			E("\u54e6...\u4f46\u662f","\u6211\u60f3\u4e0d\u5230\u8981\u8b1b\u4ec0\u9ebc","\u7b97\u4e86");
			/**< 哦...但是 我想不到要講什麼 算了 */
			E("\u62ac\u69d3","\u66ab\u6642\u653e\u904e\u4f60","\u63b0\u63b0");
			/**< E("抬槓","暫時放過你","掰掰"); */
            int tmp = 3;
            while(tmp--){
				E("\u63b0\u63b0~~~","\u63b0\u63b0~~~","\u63b0\u63b0~~~~~");
				/**< 掰掰~~~ 掰掰~~~ 掰掰~~~~~ */
            }
			return;
		}else{
			E("\u4f60\u9a19\u6211!!!!","\u4f60\u9a19\u6211!!!!","\u4f60\u9a19\u6211!!!!");
			/**< E("你騙我!!!!","你騙我!!!!","你騙我!!!!"); */
		}
	}else{
		ret = Q("\u70ba\u4ec0\u9ebc!!!!","\u70ba\u4ec0\u9ebc\u6703\u9019\u6a23","\u9084\u662f\u4f60\u5077\u5077\u4fee\u6539\u6211\u5011\u7684\u5716\u7247\uff1f");
		/**< ret = Q("為什麼!!!!","為什麼會這樣","還是你偷偷修改我們的圖片？"); */
		if(!ret){
			E("\u4f60\u9a19\u6211!!!!","\u4f60\u9a19\u6211!!!!","\u4f60\u9a19\u6211!!!!");
			/**< E("你騙我!!!!","你騙我!!!!","你騙我!!!!"); */
		}
	}
	E("\u6211\u5f88\u751f\u6c23","\u56e0\u70ba\u4f60\u628a\u6211\u7684\u7a0b\u5f0f\u5f04\u58de\u4e86","\u7f70\u4f60\u8981\u6309 1000 \u4e0bEnter\u624d\u80fd\u7d50\u675f");
	/**< 我很生氣 因為你把我的程式弄壞了 罰你要按 1000 下Enter才能結束 */
	int k=1000;
	while(--k) {
		char tmp[100];
		sprintf(tmp,"\u7f70\u4f60\u518d\u6309 %d \u4e0bEnter\u624d\u80fd\u7d50\u675f",k);
		E("\u6211\u5f88\u751f\u6c23","\u56e0\u70ba\u4f60\u628a\u6211\u7684\u7a0b\u5f0f\u5f04\u58de\u4e86",tmp);
		/**< 我很生氣 因為你把我的程式弄壞了 罰你再按 k 下Enter才能結束 */
	}
}


struct _Resources Res;
