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

void lightSpotContainerInit(Map* map);
void freeLightSpotContainer(Map* map);


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


LightSpotContainer* getLightSpotContainer_xy(Map* map,int index_x,int index_y);
LightSpotContainer* getLightSpotContainer_P(Map* map,LightSpotPos lSpP);
int getContainerIndex(Map* map,int pos);
LightSpotPos getLightSpotPos(Map* map,int x,int y);
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
void Eated_LightSpot(LightSpotContainer* container,int i);

/** @brief 探測附近有沒有lightspot
 * 有的話就會吸過來吃掉
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
