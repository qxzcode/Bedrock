//
//  Air.h
//  Bedrock
//
//  Created by Quinn on 9/21/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Block.h"

class Air: public Block {
public:
	Air():Block("Air", 0, -1, false, true) {}
};