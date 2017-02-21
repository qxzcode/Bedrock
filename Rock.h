//
//  Rock.h
//  Bedrock
//
//  Created by Quinn on 4/6/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Item.h"

class Rock: public Item {
public:
	Rock();
	
	virtual void draw(float x, float y, float z, int state);
	
protected:
	float texCoords[2*4*6];
	
};