#ifndef LIGHTSPOT_H
#define LIGHTSPOT_H
/**< �Ҧ���G�I���ާ@ */
#include "Structures.h"
#include "Map.h"


void lightSpotContainerInit(Map* map);
void freeLightSpotContainer(Map* map);
LightSpot Create_LightSpot(int mapSize);

LightSpot Create_LightSpot_xy(int x,int y);
LightSpot Create_LightSpot_xyc(int x,int y,int color);

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




#endif // LIGHTSPOT_H
