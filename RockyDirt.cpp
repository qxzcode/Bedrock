//
//  RockyDirt.cpp
//  Bedrock
//
//  Created by Quinn on 11/15/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "RockyDirt.h"

#include <stdlib.h>

static inline bool isGrass(blockID block) {
	return block==GRASS_ID || block==TALL_GRASS_ID || block==FLOWER_GRASS_ID;
}

blockTexCoords& RockyDirt::getTexCoords(blockTexCoords& tcFinal, int x, int y, int z, World* w) {
	blockTexCoords tcGrass = texCoords;
	
	sideTexCoords grass = Block::block(GRASS_ID)->texCoords[0];
	if (isGrass(w->getBlock(x+1, y, z)))	tcGrass[0] = grass;
	if (isGrass(w->getBlock(x-1, y, z)))	tcGrass[1] = grass;
	if (isGrass(w->getBlock(x, y+1, z)))	tcGrass[2] = grass;
	if (isGrass(w->getBlock(x, y, z+1)))	tcGrass[4] = grass;
	if (isGrass(w->getBlock(x, y, z-1)))	tcGrass[5] = grass;
	
	if (!randTex)
		return tcFinal = tcGrass;
	
	//// randomly rotate texture (so it looks like less of a grid) ////
	tcFinal = tcGrass;
	
	srandom(x + (WORLD_X*y) + (WORLD_X*WORLD_Y*z));
	for (int side = 0; side < 6; side++)
		switch (random() % 4) {
			case 0:
				tcFinal[side].c1x = tcGrass[side].c1x;
				tcFinal[side].c1y = tcGrass[side].c1y;
				tcFinal[side].c2x = tcGrass[side].c2x;
				tcFinal[side].c2y = tcGrass[side].c2y;
				tcFinal[side].c3x = tcGrass[side].c3x;
				tcFinal[side].c3y = tcGrass[side].c3y;
				tcFinal[side].c4x = tcGrass[side].c4x;
				tcFinal[side].c4y = tcGrass[side].c4y;
				break;
			case 1:
				tcFinal[side].c1x = tcGrass[side].c2x;
				tcFinal[side].c1y = tcGrass[side].c2y;
				tcFinal[side].c2x = tcGrass[side].c3x;
				tcFinal[side].c2y = tcGrass[side].c3y;
				tcFinal[side].c3x = tcGrass[side].c4x;
				tcFinal[side].c3y = tcGrass[side].c4y;
				tcFinal[side].c4x = tcGrass[side].c1x;
				tcFinal[side].c4y = tcGrass[side].c1y;
				break;
			case 2:
				tcFinal[side].c1x = tcGrass[side].c3x;
				tcFinal[side].c1y = tcGrass[side].c3y;
				tcFinal[side].c2x = tcGrass[side].c4x;
				tcFinal[side].c2y = tcGrass[side].c4y;
				tcFinal[side].c3x = tcGrass[side].c1x;
				tcFinal[side].c3y = tcGrass[side].c1y;
				tcFinal[side].c4x = tcGrass[side].c2x;
				tcFinal[side].c4y = tcGrass[side].c2y;
				break;
			case 3:
				tcFinal[side].c1x = tcGrass[side].c4x;
				tcFinal[side].c1y = tcGrass[side].c4y;
				tcFinal[side].c2x = tcGrass[side].c1x;
				tcFinal[side].c2y = tcGrass[side].c1y;
				tcFinal[side].c3x = tcGrass[side].c2x;
				tcFinal[side].c3y = tcGrass[side].c2y;
				tcFinal[side].c4x = tcGrass[side].c3x;
				tcFinal[side].c4y = tcGrass[side].c3y;
				break;
		}
	
	return tcFinal;
}