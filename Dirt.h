//
//  Dirt.h
//  Bedrock
//
//  Created by Quinn on 9/21/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Block.h"

#include "World.h"

class Dirt: public Block {
public:
	Dirt(blockID id):Block("Dirt", id, 1, true, false, true) {}
	
	virtual blockTexCoords& getTexCoords(blockTexCoords& tc, int x, int y, int z, World* w);
};