#include "draw_map.h"

#include <stdio.h>


int Position_To_Picture(int number,ALLEGRO_DISPLAY *display,int select) {
	if(select==0)
		return number/GRAPH_SIZE;               //return picture's number
	else if(select==1)
		return number%GRAPH_SIZE;               //return position in picture
	else if(select==2)
		return (GRAPH_SIZE-number%GRAPH_SIZE)>al_get_display_width(display)? al_get_display_width(display):(GRAPH_SIZE-number%GRAPH_SIZE);    //return the draw range in x axis
	else if(select==3)
		return (GRAPH_SIZE-number%GRAPH_SIZE)>al_get_display_height(display)? al_get_display_height(display):(GRAPH_SIZE-number%GRAPH_SIZE);    //return the draw range in y axis
	return -1;
}

void Draw_Map(Position position,ALLEGRO_BITMAP ***bitmap,ALLEGRO_DISPLAY *display,Map *map) {
	int pict_x_min,pict_x_max,pict_y_min,pict_y_max;    //picture number in the screen
	int x_min,x_max,y_min,y_max;                        //the screen's size
	int i,j;
	int dx=0,dy=0;          //position to draw bitmap in the screen

	x_min=position.x-al_get_display_width(display)/2;
	x_max=position.x+al_get_display_width(display)/2;
	y_min=position.y-al_get_display_height(display)/2;
	y_max=position.y+al_get_display_height(display)/2;

	pict_x_min=PTP(x_min,display,0);
	pict_x_max=PTP(x_max,display,0);
	pict_y_min=PTP(y_min,display,0);
	pict_y_max=PTP(y_max,display,0);

	for(i=pict_y_min; i<=pict_y_max; i++) {
		for(j=pict_x_min; j<=pict_x_max; j++) {
			if(i<PICTURE_NUM && i>=0 && j<PICTURE_NUM && j>=0) {
				al_draw_bitmap_region(bitmap[i][j],PTP(x_min,display,1),PTP(y_min,display,1),PTP(x_min,display,2),PTP(y_min,display,3),dx,dy,0);
			}
			Draw_Map_Border(position.x,position.y,display,map);
			dx+=PTP(x_min,display,2);
			x_min+=PTP(x_min,display,2);
		}
		dx=0;
		x_min=position.x-al_get_display_width(display)/2;
		dy+=PTP(y_min,display,3);
		y_min+=PTP(y_min,display,3);
	}
}

void Draw_Map_Border(int x,int y,ALLEGRO_DISPLAY *display,Map *map) {
	if(x+al_get_display_width(display)/2>=map->size) {
		al_draw_line(Pos(map->size,x,display,0),0,Pos(map->size,x,display,0),al_get_display_height(display),al_map_rgb(0,255,255),10.0);
	} else if(x-al_get_display_width(display)/2<=0) {
		al_draw_line(Pos(0,x,display,0),0,Pos(0,x,display,0),al_get_display_height(display),al_map_rgb(0,255,255),10.0);
	}
	if(y+al_get_display_height(display)/2>=map->size) {
		al_draw_line(0,Pos(map->size,y,display,1),al_get_display_width(display),Pos(map->size,y,display,1),al_map_rgb(0,255,255),10.0);
	} else if(y-al_get_display_height(display)/2<=0) {
		al_draw_line(0,Pos(0,y,display,1),al_get_display_width(display),Pos(0,y,display,1),al_map_rgb(0,255,255),10.0);
	}
}



int Pos(int number,int center,ALLEGRO_DISPLAY *display,int select) {
	if(select==0)
		return number-center+(al_get_display_width(display)>>1);
	else if(select==1)
		return number-center+(al_get_display_height(display)>>1);
	return -1;
}


