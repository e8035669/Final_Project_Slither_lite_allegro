#include "draw_snake.h"




void Draw_Snake(Snake *snake,ALLEGRO_BITMAP *snake_body,ALLEGRO_BITMAP *snake_head,ALLEGRO_DISPLAY *display) {
	int i;
	int picture_size;
	double center_x,center_y;
	Body *current_body;
	double current_x,current_y;

	picture_size=Picture_Size(snake->length);
	center_x=snake->head->current_position.x;
	center_y=snake->head->current_position.y;
	current_body=snake->tail;
	current_x=current_body->current_position.x;
	current_y=current_body->current_position.y;



	for(i=0; i<snake->length; i++) {
		if(current_body->prev==NULL) {
			Draw_Snake_Head(snake->mouse.x,snake->mouse.y,picture_size,snake_head,display);
		} else {
			if(current_x<=center_x+al_get_display_width(display)/2+picture_size/2 &&
					current_x>=center_x-al_get_display_width(display)/2-picture_size/2 &&
					current_y<=center_y+al_get_display_height(display)/2+picture_size/2 &&
					current_y>=center_y-al_get_display_height(display)/2-picture_size/2) {
				al_draw_scaled_bitmap(snake_body,0,0,SNAKE_GRAPH_SIZE,SNAKE_GRAPH_SIZE,Pos(current_x,center_x,display,0)-picture_size/2,Pos(current_y,center_y,display,1)-picture_size/2,picture_size,picture_size,0);
			}
			current_body=current_body->prev;
			current_x=current_body->current_position.x;
			current_y=current_body->current_position.y;
		}
	}
}

void Draw_Snake_Head(int mouse_x,int mouse_y,int picture_size,ALLEGRO_BITMAP *snake_head,ALLEGRO_DISPLAY *display) {
	double scaled_rate=picture_size/(SNAKE_GRAPH_SIZE*1.0);
	double theta;

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
	}
	al_draw_scaled_rotated_bitmap(snake_head,SNAKE_GRAPH_SIZE/2,SNAKE_GRAPH_SIZE/2,al_get_display_width(display)/2,al_get_display_height(display)/2,scaled_rate,scaled_rate,theta,0);

}
