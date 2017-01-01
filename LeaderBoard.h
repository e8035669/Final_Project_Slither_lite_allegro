#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kdebug/debug_c.h"

typedef struct Record {
	char name[20];
	int score;
	time_t timeStamp;
} Record;

void LeaderBoard_insertNewRecord(char[],int b);
int LeaderBoard_getRecordCount();
Record* LeaderBoard_getRecordData(int count);
int LeaderBoard_sortRecord(Record* records,int count);

#endif // LEADERBOARD_H
