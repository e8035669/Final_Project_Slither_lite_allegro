#include "Init.h"

void init() {
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
}

void initResources() {
	//al_set_new_display_refresh_rate(60);
	al_set_new_display_option(ALLEGRO_VSYNC,1,ALLEGRO_REQUIRE);
	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	Res.display = al_create_display(800,600);
	Res.pongFont = al_load_ttf_font("assets/ARCHRISTY.ttf", 70, 0);
	Res.builtinFont = al_create_builtin_font();

	al_set_target_backbuffer(Res.display);
	al_clear_to_color(al_map_rgb(96,96,96));
	al_draw_text(Res.pongFont,al_map_rgb(220,220,220),al_get_display_width(Res.display)/2,
				 al_get_display_height(Res.display)/2,ALLEGRO_ALIGN_CENTRE,"Loading...");
	Res.icon=al_load_bitmap("assets/icon.png");
	al_set_display_icon(Res.display,Res.icon);

	al_flip_display();//Show loading screen

	Res.start = al_load_bitmap( "assets/start.jpg");
	Res.start_button = al_load_bitmap( "assets/start_button.png");
	Res.start_button_blink = al_load_bitmap( "assets/start_button_blink.png");

	Res.bitmap = (ALLEGRO_BITMAP***)malloc(sizeof(ALLEGRO_BITMAP*)*PICTURE_NUM);
	int i,j;
	for(i=0; i<PICTURE_NUM; i++) {
		Res.bitmap[i]=(ALLEGRO_BITMAP **) malloc(sizeof(ALLEGRO_BITMAP *) *PICTURE_NUM);
	}
	char str[]="assets/bg/  .jpg";
	for(i=0; i<PICTURE_NUM; i++) {
		for(j=0; j<PICTURE_NUM; j++) {
			str[10]=i+'0';
			str[11]=j+'0';
			Res.bitmap[i][j]=al_load_bitmap(str);
		}
	}

	Res.snake_body = al_load_bitmap("assets/snake/snake_body.png");
	Res.snake_head = al_load_bitmap("assets/snake/snake_head.png");
	Res.lightspot = al_load_bitmap("assets/lightspot/lightspot.png");
	Res.timer = al_create_timer(1.0/60.0);
	Res.eventQueue = al_create_event_queue();
	al_register_event_source(Res.eventQueue,al_get_display_event_source(Res.display));
	al_register_event_source(Res.eventQueue,al_get_keyboard_event_source());
	al_register_event_source(Res.eventQueue,al_get_mouse_event_source());
	al_register_event_source(Res.eventQueue,al_get_timer_event_source(Res.timer));
	al_start_timer(Res.timer);
	LOG("%-30s[OK]","Allegro Resource Loading");
}

void destroy() {
	al_destroy_display(Res.display);
	al_destroy_font(Res.pongFont);
	al_destroy_font(Res.builtinFont);
	al_destroy_bitmap(Res.start);
	al_destroy_bitmap(Res.start_button);
	al_destroy_bitmap(Res.start_button_blink);
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
	al_destroy_event_queue(Res.eventQueue);
}

struct _Resources Res;
