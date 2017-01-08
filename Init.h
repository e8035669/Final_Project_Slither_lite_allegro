#ifndef INIT_H
#define INIT_H

#include <stdlib.h>
#include "define.h"
#include "kdebug/debug_c.h"
#include "atan2_dict.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

struct _Resources {
	ALLEGRO_DISPLAY* display;
	ALLEGRO_FONT *pongFont,*builtinFont;
	ALLEGRO_BITMAP *start,*start_button,*start_button_blink,*icon;
	ALLEGRO_BITMAP ***bitmap,*snake_body,*snake_head,*lightspot;
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* eventQueue;
	ALLEGRO_SAMPLE *bgMusic,*crashSound,*eatSound,*slideSound;
	ALLEGRO_MIXER* defaultMixer;
	ALLEGRO_SAMPLE_INSTANCE *slideSoundInstance;
	ALLEGRO_EVENT_SOURCE eventSource;
};

void init();
void initResources();
void destroy();


extern struct _Resources Res;
#endif // INIT_H
