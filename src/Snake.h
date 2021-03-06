#ifndef SNAKE_H
#define SNAKE_H
/**< 蛇的建立 刪除 移動 等基本動作 */
#define INIT_LENGTH 10
#include "Structures.h"


/** @brief 建立一隻蛇的Struct 並且初始化蛇的長度為10
 *	需要建立的位置 以及 蛇的名子
 * @param position Position
 * @param name char*
 * @return Snake*
 */
Snake* createSnake(Position position,char* name);

/** @brief 刪除蛇的Struct
 *	如果蛇死掉了就需要用這個function釋放記憶體
 * @param snake Snake*
 * @return void
 *
 */
void deleteSnake(Snake* snake);


/** @brief 讓蛇朝著滑鼠游標前進一步(距離5)
 *  從蛇的最尾端 複製前一個身體的位置變為自己的位置
 *	蛇的頭的位置則是使用滑鼠的位置為根據
 *  如果有按下滑鼠 可以加速(7?)
 * @param snake Snake*
 * @param cursor Mouse
 * @param speed double
 * @return void
 *
 */
void moveSnake(Snake* snake,Mouse cursor,double speed);

void aiSnakes(Snake* snakes[],int Ainumbers);

void Snake_rebirth(Snake* snakes[],int Ainumbers);

/** @brief return the speedDelta for snake
 *
 * @param speedMax int
 * @param speedMin int
 * @param accelerate int
 * @param slowDown int
 * @param speed int
 * @param speedDelta int
 * @return int
 *
 */
int snakeSpeedDelta(int speedMax,int speedMin,int accelerate,int slowDown,int speed,int speedDelta);

#endif // SNAKE_H
