#ifndef MAP_H
#define MAP_H
#include <stdlib.h>
#include "define.h"
#include "Structures.h"
#include "draw_map.h"
#include <allegro5/allegro.h>

#define TOTAL_COLOR 5



/** @brief 建立地圖的結構
 *	此地圖的結構只儲存每一個亮點(食物)的位置
 *	當蛇的頭與亮點接近時，就可以吃掉
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
