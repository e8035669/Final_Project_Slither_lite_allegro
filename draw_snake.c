#include "draw_snake.h"
#include "draw_map.h"
#include <math.h>

void Draw_Snake(Snake *snake,ALLEGRO_BITMAP *snake_body,ALLEGRO_BITMAP *snake_head,ALLEGRO_DISPLAY *display,Position center) {
	if(snake->isDead==3)return;
	int i;
	int picture_size;
	Body *current_body;
	double current_x,current_y;
	int displayW_2 = al_get_display_width(display)>>1;
	int displayH_2 = al_get_display_height(display)>>1;

	picture_size=snake->picSize;
	current_body=snake->tail;
	current_x=current_body->current_position.x;
	current_y=current_body->current_position.y;

	int upBorder = center.y+displayH_2+(picture_size>>1);
	int downBorder = center.y-displayH_2-(picture_size>>1);
	int leftBorder = center.x-displayW_2-(picture_size>>1);
	int rightBorder = center.x+displayW_2+(picture_size>>1);


	for(i=0; i<snake->length; i++) {
		if(current_x<=rightBorder && current_x>=leftBorder && current_y<=upBorder && current_y>=downBorder) {
			if(current_body->prev==NULL) {
				Draw_Snake_Head(snake,center,picture_size,snake_head,display);
				return;
			} else {
				al_draw_scaled_bitmap(snake_body,0,0,SNAKE_GRAPH_SIZE,SNAKE_GRAPH_SIZE,Pos(current_x,center.x,display,0)-picture_size/2,Pos(current_y,center.y,display,1)-picture_size/2,picture_size,picture_size,0);
			}
		} else if(current_body->prev==NULL) return;
		current_body=current_body->prev;
		current_x=current_body->current_position.x;
		current_y=current_body->current_position.y;
	}
}

void Draw_Snake_Head(Snake* snake,Position center,int picture_size,ALLEGRO_BITMAP *snake_head,ALLEGRO_DISPLAY *display) {
	double scaled_rate=picture_size/(SNAKE_GRAPH_SIZE*1.0);
	double theta = snake->headDirection+M_PI_2;
//	LOG("theta = %f",theta*180/M_PI);
	/*
		if(mouse_y==al_get_display_height(display)/2) {
			if(mouse_x>al_get_display_width(display)/2) {
				theta=M_PI/2;
			} else if(mouse_x<al_get_display_width(display)/2) {
				theta=-M_PI/2;
			}
		} else if(mouse_y<al_get_display_height(display)/2) {
			theta=atan((mouse_x-al_get_display_width(display)/2)/((mouse_y-al_get_display_height(display)/2)*-1.0));
		} else if(mouse_y>al_get_display_height(display)/2) {
			if(mouse_x>al_get_display_width(display)/2) {
				theta=atan((mouse_x-al_get_display_width(display)/2)/((mouse_y-al_get_display_height(display)/2)*-1.0))+M_PI;
			} else if(mouse_x<al_get_display_width(display)/2) {
				theta=atan((mouse_x-al_get_display_width(display)/2)/((mouse_y-al_get_display_height(display)/2)*-1.0))-M_PI;
			} else if(mouse_x==al_get_display_width(display)/2) {
				theta=-M_PI;
			}
		}*/
	al_draw_scaled_rotated_bitmap(snake_head,SNAKE_GRAPH_SIZE/2,SNAKE_GRAPH_SIZE/2,Pos(snake->head->current_position.x,center.x,display,0),Pos(snake->head->current_position.y,center.y,display,1),scaled_rate,scaled_rate,theta,0);

}
