//
//  Grass.h
//  Bedrock
//
//  Created by Quinn on 11/11/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Block.h"

static inline bool isDirt(blockID block) {
	return block==DIRT_ID || block==ROCKY_DIRT_ID;
}
static inline bool isClay(blockID block) {
	return block==CLAY_ID || block==ROCKY_CLAY_ID;
}

class Grass: public Block {
public:
	Grass(blockID id):Block("Grass", id, 3, false, true, false) {}
	
	virtual void onBlockUpdate(World* w, int x, int y, int z, int ux, int uy, int uz) {
		if (!(isDirt(w->getBlock(x+1, y, z)) ||
			isDirt(w->getBlock(x-1, y, z)) ||
			isDirt(w->getBlock(x, y, z+1)) ||
			isDirt(w->getBlock(x, y, z-1)) ||
			isDirt(w->getBlock(x, y-1, z))))
			w->setBlockWithNotify(x, y, z, 0);
	}
};