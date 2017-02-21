//
//  RockyClay.h
//  Bedrock
//
//  Created by Quinn on 11/15/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Block.h"

class RockyClay: public Block {
public:
	RockyClay(blockID id):Block("Rocky Clay", id, 10, true, false, true) {}
};