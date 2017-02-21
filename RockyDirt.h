//
//  RockyDirt.h
//  Bedrock
//
//  Created by Quinn on 10/17/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Block.h"

class RockyDirt: public Block {
public:
	RockyDirt(blockID id):Block("Rocky Dirt", id, 2, true, false, true) {}
	
	virtual blockTexCoords& getTexCoords(blockTexCoords& tc, int x, int y, int z, World* w);
};