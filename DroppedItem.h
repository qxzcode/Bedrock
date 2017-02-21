//
//  DroppedItem.h
//  Bedrock
//
//  Created by Quinn on 4/6/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Entity.h"
#include "Item.h"

class DroppedItem: public Entity {
public:
	DroppedItem(float x, float y, float z, World * w, Item* item):Entity(x,y,z,w,EXPAND_VEC3(item->collisionSize)),item(item) {}
	std::string name(){return item->name;}
	EntityType type(){return EntityType::DROPPED_ITEM;}
	
	void update(long dt);
	void draw() {
		glPushAttrib(GL_TEXTURE_BIT|GL_CURRENT_BIT);
		item->draw(x, y, z, DROPPED);
		glPopAttrib();
	}
	
	Item * item;
};