#ifndef BODYSTACK_H
#define BODYSTACK_H
/**< �D������B�z�򥻰ʧ@ */
#include "Structures.h"
#include <assert.h>
#include <stdlib.h>


/** @brief ���^"�D"���Y
 * @param snake Snake*
 * @return Body*
 */
Body* Body_getHead(Snake* snake);

/** @brief ���^"�D"������
 * @param snake Snake*
 * @return Body*
 */
Body* Body_getTail(Snake* snake);

/** @brief ���X"�D"���䤤�@��(���I��)
 * �q"�Y"�� 0 �}�l��_ �p�G�Ǧ^NULL ��ܤ��s�b
 * @param snake Snake*
 * @param which int
 * @return Body*
 *
 */
Body* Body_getBody(Snake* snake,int which);
//int Body_push(Snake* snake,Body* body);
//Body* Body_pop(Snake* snake);


/** @brief �� �D �������ܪ�1
 * @param snake Snake*
 * @return int
 */
int Snake_beLonger(Snake* snake);

/** @brief �� �D �������ܵu 1
 * �p�G�D�����פw�g�� ��l����(10) ,�N����A�Y�u�Breturn 0
 * @param snake Snake*
 * @return int
 */
int Snake_beShorter(Snake* snake);

/** @brief �ˬd�D�����׬O�_���T
 *	(debug��)
 * @param snake Snake*
 * @return void
 *
 */
void Snake_lengthCheck(Snake* snake);


int Picture_Size(int length);
#endif // BODYSTACK_H
