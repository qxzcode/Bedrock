//
//  Item.h
//  Bedrock
//
//  Created by Quinn on 4/6/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include <string>

#include "util.h"

enum ItemState {
	DROPPED, HELD_LEFT, HELD_RIGHT
};

class Item {
public:
	virtual void update(long dt) {}
	virtual void draw(float x, float y, float z, int state) = 0;
	
	std::string name;
	util::vec3f collisionSize;
	
protected:
	Item(std::string name, util::vec3f size):name(name),collisionSize(size) {}
	
};