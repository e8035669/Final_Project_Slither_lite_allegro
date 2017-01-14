#include "Lightspot.h"


void lightSpotContainerInit(Map* map) {
	map->lightSpotContainers = calloc(sizeof(LightSpotContainer),map->containerSize*map->containerSize);
	int i,j;
	for(i=0; i<map->containerSize; i++) {
		for(j=0; j<map->containerSize; j++) {
			map->lightSpotContainers[i*map->containerSize+j].writeMutex = al_create_mutex();
			map->lightSpotContainers[i*map->containerSize+j].lightSpotSize = 10;
			map->lightSpotContainers[i*map->containerSize+j].lightSpotLength = 0;
			map->lightSpotContainers[i*map->containerSize+j].lightSpot = calloc(sizeof(LightSpot),10);
		}
	}
}
void freeLightSpotContainer(Map* map) {
	int i,j;
	for(i=0; i<map->containerSize; i++) {
		for(j=0; j<map->containerSize; j++) {
			free(map->lightSpotContainers[i*map->containerSize+j].lightSpot);
		}
	}
	free(map->lightSpotContainers);
	map->lightSpotContainers = NULL;
}

LightSpot Create_LightSpot(int mapSize) {
	return Create_LightSpot_xyc(rand()%(mapSize),rand()%(mapSize),(rand()%TOTAL_COLOR)+1);
}


LightSpot Create_LightSpot_xy(int x,int y) {
	return Create_LightSpot_xyc(x,y,(rand()%TOTAL_COLOR)+1);
}

LightSpot Create_LightSpot_xyc(int x,int y,int color) {
	LightSpot lightspot;
	lightspot.x=x;
	lightspot.y=y;
	lightspot.color=color;
	return lightspot;
}


void Increase_LightSpotSize(LightSpotContainer* container) {
	if(container->lightSpotSize - container->lightSpotLength < 2) {
		LightSpot* tmp = realloc(container->lightSpot,sizeof(LightSpot)*(container->lightSpotSize)*2);
		if(tmp) {
			container->lightSpot = tmp;
			int i;
			LightSpot emptyLsp = Create_LightSpot_xyc(0,0,0);
			for(i=container->lightSpotSize; i<container->lightSpotSize * 2; i++) {
				container->lightSpot[i] = emptyLsp;
			}
			container->lightSpotSize *=2;
		}
	}
}

LightSpotContainer* getLightSpotContainer_xy(Map* map,int index_x,int index_y) {
	if(index_x>=0 && index_x<map->containerSize && index_y>=0 && index_y<map->containerSize) {
		return (map->lightSpotContainers)+(index_y*map->containerSize+index_x);
	} else {
		return NULL;
	}
}
LightSpotContainer* getLightSpotContainer_P(Map* map,LightSpotPos lSpP) {
	return getLightSpotContainer_xy(map,lSpP.index_x,lSpP.index_y);
}

int getContainerIndex(Map* map,int pos) {
	if(pos<0)pos = 0;
	if(pos>=map->size)pos = map->size-1;
	return pos/map->containerSize;
}

LightSpotPos getLightSpotPos(Map* map,int x,int y) {
	int index_x = getContainerIndex(map,x);
	int index_y = getContainerIndex(map,y);
	LightSpotPos lSpP = {index_x,index_y};
	return lSpP;
}

void Put_LightSpot(Map* map,LightSpot lSp) {
	LightSpotContainer* container = getLightSpotContainer_P(map,getLightSpotPos(map,lSp.x,lSp.y));
	if(container) {
		al_lock_mutex(container->writeMutex);
		Increase_LightSpotSize(container);
		int i;
		for(i=0; i<container->lightSpotSize; i++) {
			if(container->lightSpot[i].color == 0) {
				container->lightSpot[i] = lSp;
				container->lightSpotLength += 1;
				break;
			}
		}
		al_unlock_mutex(container->writeMutex);
	}

}

void Eated_LightSpot(LightSpotContainer* container,int i) {
	if(container)if(container->lightSpot[i].color) {
			al_lock_mutex(container->writeMutex);
			container->lightSpot[i] = Create_LightSpot_xyc(0,0,0);
			container->lightSpotLength--;
			al_unlock_mutex(container->writeMutex);
		}
}
