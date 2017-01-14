#ifndef BODYSTACK_H
#define BODYSTACK_H
/**< 蛇的身體處理基本動作 */
#include "Structures.h"
#include <assert.h>
#include <stdlib.h>


/** @brief 取回"蛇"的頭
 * @param snake Snake*
 * @return Body*
 */
Body* Body_getHead(Snake* snake);

/** @brief 取回"蛇"的尾巴
 * @param snake Snake*
 * @return Body*
 */
Body* Body_getTail(Snake* snake);

/** @brief 取出"蛇"的其中一塊(有點噁)
 * 從"頭"為 0 開始算起 如果傳回NULL 表示不存在
 * @param snake Snake*
 * @param which int
 * @return Body*
 *
 */
Body* Body_getBody(Snake* snake,int which);
//int Body_push(Snake* snake,Body* body);
//Body* Body_pop(Snake* snake);


/** @brief 讓 蛇 的長度變長1
 * @param snake Snake*
 * @return int
 */
int Snake_beLonger(Snake* snake);

/** @brief 讓 蛇 的長度變短 1
 * 如果蛇的長度已經為 初始長度(10) ,就不能再縮短且return 0
 * @param snake Snake*
 * @return int
 */
int Snake_beShorter(Snake* snake);

/** @brief 檢查蛇的長度是否正確
 *	(debug用)
 * @param snake Snake*
 * @return void
 *
 */
void Snake_lengthCheck(Snake* snake);


int Picture_Size(int length);
#endif // BODYSTACK_H
