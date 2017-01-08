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

void lightSpotContainerInit(Map* map);
void freeLightSpotContainer(Map* map);


/** @brief �إ߷s��lightspot
 *
 * @param mapSize int
 * @return LightSpot
 *
 */
LightSpot Create_LightSpot(int mapSize);

LightSpot Create_LightSpot_xyc(int x,int y,int color);

LightSpot Create_LightSpot_xy(int x,int y);
/** @brief draw the lightspot
*   @param map Map*
*          snake Snake*
*          lightspot ALLEGRO_BITMAP*
*   @return void
*
*/
void Draw_LightSpot(Map *map,Snake *snake,ALLEGRO_BITMAP *lightspot,ALLEGRO_DISPLAY *display);


LightSpotContainer* getLightSpotContainer_xy(Map* map,int index_x,int index_y);
LightSpotContainer* getLightSpotContainer_P(Map* map,LightSpotPos lSpP);
int getContainerIndex(Map* map,int pos);
LightSpotPos getLightSpotPos(Map* map,int x,int y);
/** @brief �s�W�o��lightspot��a�ϤW
 *
 * @param map Map*
 * @param lSp LightSpot
 * @return void
 *
 */
void Put_LightSpot(Map* map,LightSpot lSp);

/** @brief ����(�Y)���Y�@��lightspot
 *
 * @param map Map*
 * @param i int
 * @return void
 *
 */
void Eated_LightSpot(LightSpotContainer* container,int i);

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
