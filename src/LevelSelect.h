#ifndef LEVELSELECT_H
#define LEVELSELECT_H
/**< Ãö¥d¿ï¾Ü */
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"

typedef struct Button{
    int mouseOn;
    int width;
    int height;
    int startX;
    int startY;
}Button;

void drawButton(int levelNum,Button *button,ALLEGRO_FONT *font,ALLEGRO_FONT *mouseOnFont);

void setMouseOn(const ALLEGRO_MOUSE_EVENT *eventMouse,Button *button,int levelNum);

int mouseClick(const ALLEGRO_MOUSE_EVENT *eventMouse,Button *button,int levelNum);

#endif // LEVELSELECT_H
