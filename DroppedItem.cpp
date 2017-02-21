//
//  DroppedItem.cpp
//  Bedrock
//
//  Created by Quinn on 4/6/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "DroppedItem.h"

void DroppedItem::update(long dt) {
	item->update(dt);
	
	float dt_f = dt/1000.0f;
	
	ySpeed -= dt_f*gravity;
	
	x += dt_f*xSpeed;
	y += dt_f*ySpeed;
	z += dt_f*zSpeed;
	
	collisionResult cr = doCollide();
	if (cr.x) xSpeed = 0;
	if (cr.y) ySpeed = 0;
	if (cr.z) zSpeed = 0;
	if (cr.x||cr.y||cr.z) {
		xSpeed /= (dt_f*friction) + 1.0f;
		zSpeed /= (dt_f*friction) + 1.0f;
	}
}