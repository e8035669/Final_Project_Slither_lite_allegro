#include "draw_lightspot.h"

void Draw_LightSpot(Map *map,Snake *snake,ALLEGRO_BITMAP *lightspot,ALLEGRO_DISPLAY *display) {
	int i,j;
	int displayHalf_W=al_get_display_width(display)>>1;
	int displayHalf_H=al_get_display_height(display)>>1;

	Position p = snake->head->current_position;
	LightSpotPos pos1 = getLightSpotPos(map,p.x-displayHalf_W,p.y-displayHalf_H);
	LightSpotPos pos2 = getLightSpotPos(map,p.x+displayHalf_W,p.y+displayHalf_H);


	for(i=pos1.index_x; i<=pos2.index_x; i++) {
		for(j=pos1.index_y; j<=pos2.index_y; j++) {
			LightSpotContainer* container = getLightSpotContainer_xy(map,i,j);
			if(container) {
				int k;
				for(k=0; k<container->lightSpotSize; k++) {
					switch (container->lightSpot[k].color) {
						case 1:
							al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(255,0,0),0,0,50,50,container->lightSpot[k].x-p.x+displayHalf_W,container->lightSpot[k].y-p.y+displayHalf_H,15,15,0);
							break;
						case 2:
							al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(0,255,0),0,0,50,50,container->lightSpot[k].x-p.x+displayHalf_W,container->lightSpot[k].y-p.y+displayHalf_H,15,15,0);
							break;
						case 3:
							al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(0,0,255),0,0,50,50,container->lightSpot[k].x-p.x+displayHalf_W,container->lightSpot[k].y-p.y+displayHalf_H,15,15,0);
							break;
						case 4:
							al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(255,0,255),0,0,50,50,container->lightSpot[k].x-p.x+displayHalf_W,container->lightSpot[k].y-p.y+displayHalf_H,15,15,0);
							break;
						case 5:
							al_draw_tinted_scaled_bitmap(lightspot,al_map_rgb(0,255,255),0,0,50,50,container->lightSpot[k].x-p.x+displayHalf_W,container->lightSpot[k].y-p.y+displayHalf_H,15,15,0);
							break;
					}
				}
			}
		}
	}
}
