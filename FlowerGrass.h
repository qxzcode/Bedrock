//
//  FlowerGrass.h
//  Bedrock
//
//  Created by Quinn on 1/10/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "TallGrass.h"

#include "util.h"

class FlowerGrass: public TallGrass {
public:
	FlowerGrass(blockID id):TallGrass(id, 5, "Flower Grass") {}
	
	static const util::vec3f petalColors[], centerColors[];
	static const int numColors = 6;
};