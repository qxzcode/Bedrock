//
//  Stone.h
//  Bedrock
//
//  Created by Quinn on 10/17/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Block.h"

class Stone: public Block {
public:
	Stone(blockID id):Block("Stone", id, 0, true, false, true) {}
};