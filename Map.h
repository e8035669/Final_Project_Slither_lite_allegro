#ifndef MAP_H
#define MAP_H
#include <stdlib.h>
#include "define.h"
#include "Structures.h"
#include "draw_map.h"
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
LightSpot Create_LightSpot(void);


/** @brief draw the lightspot
*   @param map Map*
*          snake Snake*
*          lightspot ALLEGRO_BITMAP*
*   @return void
*
*/
void Draw_LightSpot(Map *map,Snake *snake,ALLEGRO_BITMAP *lightspot);


/** @brief increase the map->lightSpotSize = "size"
*   @param map Map*
*          size int
*   @return void
*/
void Increase_LightSpotSize(Map *map,int size);


#endif // MAP_H
