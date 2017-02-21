//
//  Water.h
//  Bedrock
//
//  Created by Quinn on 9/27/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Block.h"

class Water: public Block {
public:
	Water(blockID id):Block("Water", id, 8, false, true, true) {}
};