#ifndef _DRAW_MAP_H_
#define _DRAW_MAP_H_

#include "define.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>


//load the 2D bitmap pointer , picture is in the file "bg"
void My_Allegro_Load_Map(ALLEGRO_BITMAP ***bitmap);

/*
*  select=0 , return the picture number according to number
*   select=1 , return the picture coordinate according to number
*   select=2 , return the x axis draw range
*   select=3 , return the y axis draw range
*   return -1 means failed
*/
int Position_To_Picture(int number,int select);                                     //position to picture

//parameter x,y is the position in the map , bitmap is bhe background's bitmap
void Draw_Map(int x,int y,ALLEGRO_BITMAP ***bitmap);

//destroy the 2D bitmap pointer
void My_Allegro_Destroy_Map(ALLEGRO_BITMAP ***bitmap);


//draw the map border
void Draw_Map_Border(int x,int y);


/*  calculate the graph's (x,y) in the display
*
*   select = 0 ,return the x position
*   select = 1 ,return the y position
*/
int Pos(int number,int center,int select);

#define PTP(number,select) Position_To_Picture(number,select)


#endif // _DRAW_MAP_H_
