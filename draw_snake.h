#ifndef _DRAW_SNAKE_H_
#define _DRAW_SNAKE_H_

#include "define.h"
#include "Structures.h"
#include "draw_map.h"
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>


//draw snake in the middle of screen
void Draw_Snake(Snake *snake,ALLEGRO_BITMAP *snake_body,ALLEGRO_BITMAP *snake_head,ALLEGRO_DISPLAY *display,double center_x,double center_y);

void Draw_Snake_Head(Snake* snake,int center_x,int center_y,int picture_size,ALLEGRO_BITMAP *snake_head,ALLEGRO_DISPLAY *display);

#endif // _DRAW_SNAKE_H_
