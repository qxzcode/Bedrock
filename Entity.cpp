//
//  Entity.cpp
//  Bedrock
//
//  Created by Quinn on 9/27/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "Entity.h"

#include "util.h"

#include "math.h"

collisionResult Entity::doCollide() {/*
	float dx = x-lastX, dy = y-lastY, dz = z-lastZ;
	x = lastX; y = lastY; z = lastZ;
	for (;				dx>0 && dy>0 && dz>0;
						
						x+=dx>COLLIDE_STEP?COLLIDE_STEP:dx,
						y+=dy>COLLIDE_STEP?COLLIDE_STEP:dy,
						z+=dz>COLLIDE_STEP?COLLIDE_STEP:dz,
						dx-=COLLIDE_STEP,dy-=COLLIDE_STEP,dz-=COLLIDE_STEP) {//*/
	x = util::mod(x+WORLD_X, WORLD_X);
	y = util::mod(y+WORLD_Y, WORLD_Y);
	z = util::mod(z+WORLD_Z, WORLD_Z);
	
	
	float newX = x, newY = y, newZ = z;
	collisionResult result;
	
	x = lastX; y = lastY; z = lastZ;
	
	if (isColliding()) {
		x = newX;
		y = newY;
		z = newZ;
	} else {
		x = newX;
		if (isColliding()) {
			x = newX>lastX? floorf(x+rx)-rx : ceilf(x-rx)+rx;
			result.x = newX>lastX?1:-1;
		}
		y = newY;
		if (isColliding()) {
			y = newY>lastY? floorf(y+ry)-ry : ceilf(y-ry)+ry;
			result.y = newY>lastY?1:-1;
		}
		z = newZ;
		if (isColliding()) {
			z = newZ>lastZ? floorf(z+rz)-rz : ceilf(z-rz)+rz;
			result.z = newZ>lastZ?1:-1;
		}
	}
	
	lastX = x;
	lastY = y;
	lastZ = z;
	
	return result;
	//}
}

bool Entity::isColliding() {
	util::AABB myBB{x, y, z, rx, ry, rz};
	util::AABB blockBB{0, 0, 0, 0.5, 0.5, 0.5};
	
	int minX = floorf(this->x - rx),
	minY = floorf(this->y - ry),
	minZ = floorf(this->z - rz),
	maxX = ceilf(this->x + rx),
	maxY = ceilf(this->y + ry),
	maxZ = ceilf(this->z + rz);
	for (int x = minX; x <= maxX; x++) {
		blockBB.cx = x + 0.5f;
		for (int y = minY; y <= maxY; y++) {
			blockBB.cy = y + 0.5f;
			for (int z = minZ; z <= maxZ; z++) {
				if (!Block::block(world->getBlock(x, y, z))->isSolid) continue;
				blockBB.cz = z + 0.5f;
				if (util::intersects(myBB, blockBB)) {
					return true;
				}
			}
		}
	}
	
	return false;
}