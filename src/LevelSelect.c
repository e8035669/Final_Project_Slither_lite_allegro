#include "LevelSelect.h"
#include "allegro5/allegro_primitives.h"

void drawButton(int levelNum,Button *button,ALLEGRO_FONT *font,ALLEGRO_FONT *mouseOnFont) {
	int i;
	for(i=0; i<levelNum; i++) {
		al_draw_filled_rounded_rectangle(button[i].startX, button[i].startY,
								 button[i].startX+button[i].width, button[i].startY+button[i].height,20,20, al_map_rgba(0,0,255,87));
		if(button[i].mouseOn==0)
			al_draw_textf(font, al_map_rgb(0,255,0),
						  button[i].startX+(button[i].width>>1), button[i].startY+(button[i].height>>1)-(al_get_font_ascent(font)>>1),
						  ALLEGRO_ALIGN_CENTER, "%d", i+1);
		else if(button[i].mouseOn==1)
			al_draw_textf(mouseOnFont, al_map_rgb(0,255,0),
						  button[i].startX+(button[i].width>>1), button[i].startY+(button[i].height>>1)-(al_get_font_ascent(mouseOnFont)>>1),
						  ALLEGRO_ALIGN_CENTER, "%d", i+1);
	}
}


void setMouseOn(const ALLEGRO_MOUSE_EVENT *eventMouse,Button *button,int levelNum) {
	int i;
	for(i=0; i<levelNum; i++) {
		if(eventMouse->x > button[i].startX && eventMouse->x < button[i].startX+button[i].width &&
				eventMouse->y > button[i].startY && eventMouse->y < button[i].startY+button[i].height) {
			button[i].mouseOn=1;
			break;
		} else button[i].mouseOn=0;
	}
	for(i=i+1; i<levelNum; i++) {
		button[i].mouseOn=0;
	}
}


int mouseClick(const ALLEGRO_MOUSE_EVENT *eventMouse,Button *button,int levelNum) {
	int i;
	for(i=0; i<levelNum; i++) {
		if(eventMouse->x > button[i].startX && eventMouse->x < button[i].startX+button[i].width &&
				eventMouse->y > button[i].startY && eventMouse->y < button[i].startY+button[i].height) {
			return i+1;
		}
	}
	return 0;
}
