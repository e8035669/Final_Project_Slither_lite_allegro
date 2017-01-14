#ifndef COMPUTETASK_H
#define COMPUTETASK_H
/**< �M�������p��q�j���u�@ */
#include "Structures.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro.h"
#define EAT_EVENT_TYPE ALLEGRO_GET_EVENT_TYPE('E','A','T','S')
#define DEAD_EVENT ALLEGRO_GET_EVENT_TYPE('D','E','A','D')


/** @brief �������񦳨S��lightspot
 * �����ܴN�|�l�L�ӦY��
 * @param Body_getHead Body*
 * @param map Map*
 * @param snake Snake*
 * @return void
 *
 */
void detectLightSpot(Map* map,Snake* snake,ALLEGRO_SAMPLE *eat,ALLEGRO_EVENT_SOURCE *eventSource);


void outdeath(Snake* snakes[],int Ainumbers,ALLEGRO_EVENT_SOURCE* eventSrc);
void bodysdeath(Snake* snakes[],int Ainumbers,Map* map,int select,ALLEGRO_EVENT_SOURCE* eventSrc);


#endif // COMPUTETASK_H

