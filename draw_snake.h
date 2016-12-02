#ifndef _DRAW_SNAKE_H_
#define _DRAW_SNAKE_H_

#include "define.h"
#include "Structures.h"
#include "draw_map.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

//return the snake graph's size according to the length
int Picture_Size(int length);

//draw snake in the middle of screen
void Draw_Snake(Snake *snake,ALLEGRO_BITMAP *snake_body,ALLEGRO_BITMAP *snake_head);

#endif // _DRAW_SNAKE_H_
