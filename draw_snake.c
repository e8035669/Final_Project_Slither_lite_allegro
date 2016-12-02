#include "draw_snake.h"

int Picture_Size(int length)
{
    return (int)length*1.5;
}


void Draw_Snake(Snake *snake,ALLEGRO_BITMAP *snake_body,ALLEGRO_BITMAP *snake_head)
{
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



    for(i=0;i<snake->length;i++){
        if(current_body->prev==NULL){
            al_draw_scaled_bitmap(snake_head,0,0,SNAKE_GRAPH_SIZE,SNAKE_GRAPH_SIZE,Pos(current_x,center_x,0)-picture_size/2,Pos(current_y,center_y,1)-picture_size/2,picture_size,picture_size,0);
        }
        else{
            al_draw_scaled_bitmap(snake_body,0,0,SNAKE_GRAPH_SIZE,SNAKE_GRAPH_SIZE,Pos(current_x,center_x,0)-picture_size/2,Pos(current_y,center_y,1)-picture_size/2,picture_size,picture_size,0);
            current_body=current_body->prev;
            current_x=current_body->current_position.x;
            current_y=current_body->current_position.y;
        }
    }
}
