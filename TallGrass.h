//
//  TallGrass.h
//  Bedrock
//
//  Created by Quinn on 11/9/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Block.h"

#include "Grass.h"
#include <stdlib.h>

class TallGrass: public Block {
public:
	TallGrass(blockID id, texture texture, std::string name):Block(name, id, 3, false, true, false) {
		// calc side tex
		float texXn = (1.0f/BLOCK_SHEET_WIDTH) * float(texture % BLOCK_SHEET_WIDTH),
			  texYn = (1.0f/BLOCK_SHEET_WIDTH) * float(texture / BLOCK_SHEET_WIDTH),
			  texXp = texXn + (1.0f/BLOCK_SHEET_WIDTH),
			  texYp = texYn + (1.0f/BLOCK_SHEET_WIDTH);
		texCoords[1] = {texXn, texYp, texXp, texYp, texXp, texYn, texXn, texYn};
	}
	TallGrass(blockID id):TallGrass(id, 4, "Tall Grass") {}
	
	virtual void onBlockUpdate(World* w, int x, int y, int z, int ux, int uy, int uz) {
		if (!isDirt(w->getBlock(x, y-1, z))) {
			if (!(isDirt(w->getBlock(x+1, y, z)) ||
				  isDirt(w->getBlock(x-1, y, z)) ||
				  isDirt(w->getBlock(x, y, z+1)) ||
				  isDirt(w->getBlock(x, y, z-1))))
				w->setBlockWithNotify(x, y, z, 0);
			else w->setBlockWithNotify(x, y, z, GRASS_ID);
		}
	}
	
	sideTexCoords getSideTexCoords(int x, int y, int z) {
		//// randomly flip texture (so it looks like less of a grid) ////
		sideTexCoords tc = texCoords[1];
		
		srandom(x + (WORLD_X*y) + (WORLD_X*WORLD_Y*z));
		if (random() % 2) {
			tc.c1x = texCoords[1].c2x;
			tc.c2x = texCoords[1].c1x;
			tc.c3x = texCoords[1].c4x;
			tc.c4x = texCoords[1].c3x;
		}
		
		return tc;
	}
};