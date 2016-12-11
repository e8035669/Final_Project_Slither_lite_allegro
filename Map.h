#ifndef MAP_H
#define MAP_H
#include <stdlib.h>
#include <math.h>
#include "define.h"
#include "Structures.h"
#include "draw_map.h"
#include "Snake.h"
#include <allegro5/allegro.h>

#define TOTAL_COLOR 5



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

/** @brief create the lightspot
*
*   @return Lightspot
*
*/
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


/** @brief increase the map->lightSpotSize = "size"
*   @param map Map*
*          size int
*   @return void
*/
//void Increase_LightSpotSize(Map *map);

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
void Eated_LightSpot(Map* map,int i);

/** @brief �������񦳨S��lightspot
 * �����ܴN�|�l�L�ӦY��
 * @param Body_getHead Body*
 * @param map Map*
 * @param snake Snake*
 * @return void
 *
 */
void detectLightSpot(Body* Body_getHead, Map* map,Snake* snake);

#endif // MAP_H
