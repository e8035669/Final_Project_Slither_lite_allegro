#include "draw_map.h"

#include <stdio.h>
/*
void My_Allegro_Load_Map(ALLEGRO_BITMAP ***bitmap)
{
    int i,j;
    char str[]="assets/bg/  .jpg";
    for(i=0;i<PICTURE_NUM;i++)
        for(j=0;j<PICTURE_NUM;j++){
            str[10]=i+'0';
            str[11]=j+'0';
            bitmap[i][j]=al_load_bitmap(str);
        }
}*/



int Position_To_Picture(int number,int select) {
	if(select==0)
		return number/GRAPH_SIZE;
	else if(select==1)
		return number%GRAPH_SIZE;
	else if(select==2)
		return (GRAPH_SIZE-number%GRAPH_SIZE)>SCREEN_W? SCREEN_W:(GRAPH_SIZE-number%GRAPH_SIZE);
	else if(select==3)
		return (GRAPH_SIZE-number%GRAPH_SIZE)>SCREEN_H? SCREEN_H:(GRAPH_SIZE-number%GRAPH_SIZE);
	return -1;
}

void Draw_Map(int x,int y,ALLEGRO_BITMAP ***bitmap) {
	int pict_x_min,pict_x_max,pict_y_min,pict_y_max;
	int x_min,x_max,y_min,y_max;
	int i,j;
	int dx=0,dy=0;

	x_min=x-SCREEN_W/2;
	x_max=x+SCREEN_W/2;
	y_min=y-SCREEN_H/2;
	y_max=y+SCREEN_H/2;

	pict_x_min=PTP(x_min,0);
	pict_x_max=PTP(x_max,0);
	pict_y_min=PTP(y_min,0);
	pict_y_max=PTP(y_max,0);

	for(i=pict_y_min; i<=pict_y_max; i++) {
		for(j=pict_x_min; j<=pict_x_max; j++) {
			al_draw_bitmap_region(bitmap[i][j],PTP(x_min,1),PTP(y_min,1),PTP(x_min,2),PTP(y_min,3),dx,dy,0);
			Draw_Map_Border(x,y);
			dx+=PTP(x_min,2);
			x_min+=PTP(x_min,2);
		}
		dx=0;
		x_min=x-SCREEN_W/2;
		dy+=PTP(y_min,3);
		y_min+=PTP(y_min,3);
	}
}


/*
void My_Allegro_Destroy_Map(ALLEGRO_BITMAP ***bitmap)
{
    int i,j;
    for(i=0;i<PICTURE_NUM;i++)
        for(j=0;j<PICTURE_NUM;j++)
            al_destroy_bitmap(bitmap[i][j]);

    for(i=0;i<PICTURE_NUM;i++)
        free(bitmap[i]);

    free(bitmap);
}*/



void Draw_Map_Border(int x,int y) {
	if(x+SCREEN_W/2>=X_BORDER_MAX) {
		al_draw_line(Pos(X_BORDER_MAX,x,0),0,Pos(X_BORDER_MAX,x,0),SCREEN_H,al_map_rgb(0,255,255),10.0);
	} else if(x-SCREEN_W/2<=X_BORDER_MIN) {
		al_draw_line(Pos(X_BORDER_MIN,x,0),0,Pos(X_BORDER_MIN,x,0),SCREEN_H,al_map_rgb(0,255,255),10.0);
	}
	if(y+SCREEN_H/2>=Y_BORDER_MAX) {
		al_draw_line(0,Pos(Y_BORDER_MAX,y,1),SCREEN_W,Pos(Y_BORDER_MAX,y,1),al_map_rgb(0,255,255),10.0);
	} else if(y-SCREEN_H/2<=Y_BORDER_MIN) {
		al_draw_line(0,Pos(Y_BORDER_MIN,y,1),SCREEN_W,Pos(Y_BORDER_MIN,y,1),al_map_rgb(0,255,255),10.0);
	}
}



int Pos(int number,int center,int select) {
	if(select==0)
		return number-center+SCREEN_W/2;
	else if(select==1)
		return number-center+SCREEN_H/2;
	return -1;
}
