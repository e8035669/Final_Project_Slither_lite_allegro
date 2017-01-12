#ifndef DRAW_LIGHTSPOT_H
#define DRAW_LIGHTSPOT_H

#include "allegro5/allegro.h"
#include "Structures.h"
#include "Lightspot.h"
/** @brief draw the lightspot
*   @param map Map*
*          snake Snake*
*          lightspot ALLEGRO_BITMAP*
*   @return void
*
*/
void Draw_LightSpot(Map *map,Snake *snake,ALLEGRO_BITMAP *lightspot,ALLEGRO_DISPLAY *display);




#endif // DRAW_LIGHTSPOT_H
