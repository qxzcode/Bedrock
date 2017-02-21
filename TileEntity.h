//
//  TileEntity.h
//  Bedrock
//
//  Created by Quinn on 3/15/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

class World;

class TileEntity {
public:
	TileEntity(World* w, int x, int y, int z):world(w),x(x),y(y),z(z) {}
	virtual ~TileEntity() {}
	
	virtual void update(long dt) = 0;
	
	const int x, y, z;
	World * const world;
};

#include "World.h"