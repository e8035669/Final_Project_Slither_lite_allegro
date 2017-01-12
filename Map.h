#ifndef MAP_H
#define MAP_H
#include <stdlib.h>
#include <math.h>
#include "define.h"
#include "Structures.h"
#include "draw_map.h"
#include "Snake.h"
#include <allegro5/allegro.h>
#include "allegro5/allegro_audio.h"
#include "OtherFunctions.h"
#include "Lightspot.h"
#define TOTAL_COLOR 5
#define EAT_EVENT_TYPE ALLEGRO_GET_EVENT_TYPE('E','A','T','S')


/** @brief �إߦa�Ϫ����c
 *	���a�Ϫ����c�u�x�s�C�@�ӫG�I(����)����m
 *	��D���Y�P�G�I����ɡA�N�i�H�Y��
 * @param mapSize int
 * @return Map*
 *
 */
Map* createMap(int mapSize);



/** @brief delete the map
 * @param map Map*
 * @return void
 */
void deleteMap(Map* map);





/** @brief �������񦳨S��lightspot
 * �����ܴN�|�l�L�ӦY��
 * @param Body_getHead Body*
 * @param map Map*
 * @param snake Snake*
 * @return void
 *
 */
void detectLightSpot(Map* map,Snake* snake,ALLEGRO_SAMPLE *eat,ALLEGRO_EVENT_SOURCE *eventSource);


void outdeath(Snake* snakes[]);
void bodysdeath(Snake* snakes[],Map* map,int select);
void deathAnimate(Snake* snake,Map* map,int timerCount);
void AisBrain(Snake* snakes[],Mouse mouses[]);

void mapUpdateLightSpotData(Map* map);
#endif // MAP_H
