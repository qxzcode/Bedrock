//
//  MountainGrass.h
//  Bedrock
//
//  Created by Quinn on 1/30/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "TallGrass.h"

#include "util.h"

class MountainGrass: public TallGrass {
public:
	MountainGrass(blockID id):TallGrass(id, 6, "Mountain Grass") {}
	
	virtual void onBlockUpdate(World* w, int x, int y, int z, int ux, int uy, int uz) {
		if (!isClay(w->getBlock(x, y-1, z))) {
			w->setBlockWithNotify(x, y, z, 0);
		}
	}
};