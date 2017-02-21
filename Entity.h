//
//  Entity.h
//  Bedrock
//
//  Created by Quinn on 9/27/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include <string>

class World;

#define COLLIDE_STEP 0.4 // currently unsused

struct collisionResult {
	char x = 0, y = 0, z = 0;
};

enum struct EntityType {
	PLAYER, LIVING, DROPPED_ITEM
};

class Entity {
protected:
	Entity(float x, float y, float z, World * w):x(x),y(y),z(z),world(w),lastX(x),lastY(y),lastZ(z) {}
	Entity(float x, float y, float z, World * w, float rx, float ry, float rz):x(x),y(y),z(z),world(w),lastX(x),lastY(y),lastZ(z),rx(rx),ry(ry),rz(rz) {}
public:
	virtual ~Entity() {}
	
	virtual void update(long dt) = 0;
	virtual void draw() = 0;
	virtual std::string name() = 0;
	virtual EntityType type() = 0;
	virtual void onRemoved() {}
	
	float x, y, z;
	float xSpeed = 0, ySpeed = 0, zSpeed = 0; // velocity
	float rx, ry, rz;
	float lastX, lastY, lastZ;
	/// will be removed next frame if set to true
	bool remove = false;
	
	constexpr static const float friction = 9.0f;
	
	friend class World;
protected:
	collisionResult doCollide();
	bool isColliding();
	
	World * world;
};

#include "World.h"