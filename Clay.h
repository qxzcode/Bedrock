//
//  Clay.h
//  Bedrock
//
//  Created by Quinn on 11/15/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Block.h"

class Clay: public Block {
public:
	Clay(blockID id):Block("Clay", id, 9, true, false, true) {}
};