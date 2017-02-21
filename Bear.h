//
//  Bear.h
//  Bedrock
//
//  Created by Quinn on 3/30/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Quadruped.h"

class Bear: public Quadruped {
public:
	Bear(float x, float y, float z, World * w);
	std::string name(){return"Bear"+healthStr();}
	
	virtual void update(long dt);
	virtual void draw();
	
};