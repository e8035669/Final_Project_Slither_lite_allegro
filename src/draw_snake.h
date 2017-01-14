#ifndef _DRAW_SNAKE_H_
#define _DRAW_SNAKE_H_
/**< �Ҧ��e�D���u�@ */

#include "define.h"
#include "Structures.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>


//draw snake in the middle of screen
void Draw_Snake(Snake *snake,ALLEGRO_BITMAP *snake_body,ALLEGRO_BITMAP *snake_head,ALLEGRO_DISPLAY *display,Position center);

void Draw_Snake_Head(Snake* snake,Position center,int picture_size,ALLEGRO_BITMAP *snake_head,ALLEGRO_DISPLAY *display);

#endif // _DRAW_SNAKE_H_
