#ifndef SNAKE_H
#define SNAKE_H

#define Ainumbers 500
#define INIT_LENGTH 10
#include "Structures.h"
#include "BodyStack.h"
#include "OtherFunctions.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "atan2_dict.h"

/** @brief �إߤ@���D��Struct �åB��l�ƳD�����׬�10
 *	�ݭn�إߪ���m �H�� �D���W�l
 * @param position Position
 * @param name char*
 * @return Snake*
 */
Snake* createSnake(Position position,char* name);

/** @brief �R���D��Struct
 *	�p�G�D�����F�N�ݭn�γo��function����O����
 * @param snake Snake*
 * @return void
 *
 */
void deleteSnake(Snake* snake);


/** @brief ���D�µ۷ƹ���Ыe�i�@�B(�Z��5)
 *  �q�D���̧��� �ƻs�e�@�Ө��骺��m�ܬ��ۤv����m
 *	�D���Y����m�h�O�ϥηƹ�����m���ھ�
 *  �p�G�����U�ƹ� �i�H�[�t(7?)
 * @param snake Snake*
 * @param cursor Mouse
 * @param speed double
 * @return void
 *
 */
void moveSnake(Snake* snake,Mouse cursor,double speed);

void aiSnakes(Snake* snakes[]);

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
