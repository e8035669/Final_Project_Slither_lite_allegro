#include "LeaderBoard.h"
void LeaderBoard_insertNewRecord(char name[],int score) {
	int count = LeaderBoard_getRecordCount();
	Record* records = LeaderBoard_getRecordData(count);
	LeaderBoard_sortRecord(records,count);

	Record thisRecord;
	strcpy(thisRecord.name,name);
	thisRecord.score = score;
	thisRecord.timeStamp = time(NULL);

	Record* tmp = realloc(records,sizeof(Record)*(count+1));
	if(tmp) {
		records = tmp;
	} else {
		assert(0);
	}

	records[count] = thisRecord;
	count++;
	LeaderBoard_sortRecord(records,count);
	if(count>10) {
		FILE* file = fopen("data1","wb");
		fwrite(records,sizeof(Record),10,file);
		fclose(file);
	} else {
		FILE* file = fopen("data1","wb");
		fwrite(records,sizeof(Record),count,file);
		fclose(file);
	}
}

int LeaderBoard_getRecordCount() {
	int count=0;
	fclose(fopen("data1","a"));
	FILE *fp = fopen("data1","rb");
	if(fp) {
		Record tmp;
		while (fread(&tmp,sizeof(Record),1,fp)) {
			count++;
		}
		fclose(fp);
	} else {
		count = -1;
	}
	return count;
}

Record* LeaderBoard_getRecordData(int count) {
	fclose(fopen("data1","a"));
	FILE *fp = fopen("data1","rb");
	if(fp) {
		Record* records = malloc(count*sizeof(Record));
		int inputCount = fread(records,sizeof(Record),count,fp);
		fclose(fp);
		LOG("count = %d inputCount = %d",count,inputCount);
		assert(inputCount == count);
		return records;
	} else {
		return NULL;
	}
}

int LeaderBoard_sortRecord(Record* records,int count) {
	int i=0,j=0;
	for( i = 0; i < count; i++) {
		for( j = i; j < count; j++) {
			if( records[j].score > records[i].score ) {
				Record tmp = records[j];
				records[j] = records[i];
				records[i] = tmp;
			}
		}
	}
	return 0;
}
