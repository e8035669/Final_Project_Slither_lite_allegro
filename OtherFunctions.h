#ifndef OTHERFUNCTION_H
#define OTHERFUNCTION_H
#include <math.h>
#include "Structures.h"
#include <allegro5/allegro_audio.h>


double distanceof(double x1,double y1,double x2,double y2);

Position p(double x,double y);
Mouse m(int x,int y);

void eatenMusic(ALLEGRO_EVENT event,Position center,ALLEGRO_SAMPLE *eat);

int pow2(int x);


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
#endif // OTHERFUNCTION_H
