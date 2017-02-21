//
//  Leaves.h
//  Bedrock
//
//  Created by Quinn on 3/13/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Block.h"
#include "TileEntity.h"

class LeavesTileEntity: public TileEntity {
public:
	LeavesTileEntity(World* w, int x, int y, int z):TileEntity(w,x,y,z) {}
	virtual void update(long dt) {
		if (firstUpdate) {
			int groundY = y-1;
			for (; !Block::block(world->getBlock(x,groundY-1,z))->isSolid; groundY--);
			for (int n = 0, N = random()%3; n < N; n++)
				world->addParticle(FALLING_LEAF, x+(random()/2147483647.0f), groundY+1, z+(random()/2147483647.0f), true);
			firstUpdate = false;
		} else if (random()%2400 == 0)
			world->addParticle(FALLING_LEAF, x+(random()/2147483647.0f), y, z+(random()/2147483647.0f));
	}
private:
	bool firstUpdate = true;
};

class Leaves: public Block {
public:
	Leaves(std::string name, blockID id, texture tex):Block(name, id, tex, true, false, true) {}
	
	virtual void onBlockUpdate(World* w, int x, int y, int z, int ux, int uy, int uz) {
		onVisible(w, x, y, z);
	}
	virtual void onVisible(World* w, int x, int y, int z) {
		if (Block::block(w->getBlock(x, y-1, z))->isSolid) {
			if (w->getTileEntity(x, y, z))
				w->deleteTileEntity(x, y, z);
		} else {
			if (!w->getTileEntity(x, y, z))
				w->setTileEntity(new LeavesTileEntity(w,x,y,z), x, y, z);
		}
	}
	virtual void onInvisible(World* w, int x, int y, int z) {
		w->deleteTileEntity(x, y, z);
	}
};