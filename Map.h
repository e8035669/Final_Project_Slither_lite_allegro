#ifndef MAP_H
#define MAP_H
/**< �a�Ϫ��򥻾ާ@ �إ� �R�� */
#include "Structures.h"
#define TOTAL_COLOR 5



/** @brief �إߦa�Ϫ����c
 *	���a�Ϫ����c�u�x�s�C�@�ӫG�I(����)����m
 *	��D���Y�P�G�I����ɡA�N�i�H�Y��
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
