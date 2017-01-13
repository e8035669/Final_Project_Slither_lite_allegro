#ifndef MAP_H
#define MAP_H
/**< 地圖的基本操作 建立 刪除 */
#include "Structures.h"
#define TOTAL_COLOR 5



/** @brief 建立地圖的結構
 *	此地圖的結構只儲存每一個亮點(食物)的位置
 *	當蛇的頭與亮點接近時，就可以吃掉
 * @param mapSize int
 * @return Map*
 *
 */
Map* createMap(int mapSize,int lightSpot);



/** @brief delete the map
 * @param map Map*
 * @return void
 */
void deleteMap(Map* map);


void mapUpdateLightSpotData(Map* map);
#endif // MAP_H
