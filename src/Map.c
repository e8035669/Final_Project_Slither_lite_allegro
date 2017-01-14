#include "Map.h"
#include <stdlib.h>
#include "Lightspot.h"

Map* createMap(int mapSize,int lightSpot) {
	int i;
	Map* map = malloc(sizeof(Map));
	map->size = mapSize;
	map->totalLightSpotLength = 0;
	map->totalLightSpotSize = 0;
	map->containerSize = 100;
	lightSpotContainerInit(map);
	for(i=0; i<lightSpot; i++) {
		Put_LightSpot(map,Create_LightSpot(map->size));
	}
	return map;
}

void deleteMap(Map* map) {
	freeLightSpotContainer(map);
	free(map);
}

void mapUpdateLightSpotData(Map* map) {
	int totalLightSpotSize = 0;
	int totalLightSpotLength = 0;
	int i,j;
	for(i=0; i<map->containerSize; i++) {
		for(j=0; j<map->containerSize; j++) {
			totalLightSpotSize += (map->lightSpotContainers[i*map->containerSize+j].lightSpotSize);
			totalLightSpotLength += (map->lightSpotContainers[i*map->containerSize+j].lightSpotLength);
		}
	}
	map->totalLightSpotLength = totalLightSpotLength;
	map->totalLightSpotSize = totalLightSpotSize;
}
