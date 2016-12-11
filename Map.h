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
/** @brief 建立新的lightspot
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

/** @brief 新增這個lightspot到地圖上
 *
 * @param map Map*
 * @param lSp LightSpot
 * @return void
 *
 */
void Put_LightSpot(Map* map,LightSpot lSp);

/** @brief 移除(吃)掉某一個lightspot
 *
 * @param map Map*
 * @param i int
 * @return void
 *
 */
void Eated_LightSpot(Map* map,int i);

/** @brief 探測附近有沒有lightspot
 * 有的話就會吸過來吃掉
 * @param Body_getHead Body*
 * @param map Map*
 * @param snake Snake*
 * @return void
 *
 */
void detectLightSpot(Body* Body_getHead, Map* map,Snake* snake);

#endif // MAP_H
