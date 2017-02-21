//
//  Particle.cpp
//  Bedrock
//
//  Created by Quinn on 3/13/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "Particle.h"

#include "globals.h"

#include "math.h"

Particle::Particle(int type, float x, float y, float z, bool old):type(type),x(x),y(y),z(z) {
	switch (type) {
		case FALLING_LEAF:
			life = old?random()%60000:60000;
			size = 0.125f;
			if (random()%4 == 0) {
				// green leaf
				color = {0, float(random()%30)/100.0f + 0.33f, 0};
			} else {
				// brown leaf
				static const util::vec3f
					darkBrown = {139/256.0f,69/256.0f,19/256.0f},
					lightBrown = {205/256.0f,133/256.0f,63/256.0f};
				float v = random()/2147483647.0f;
				color = lightBrown*v + darkBrown*(1-v);
			}
			float angle = (random()/2147483647.0f)*M_PI*2;
			param1 = cosf(angle);
			param2 = sinf(angle);
			param3 = -1;
			break;
	}
}

void Particle::update(long dt) {
#define dist (curUnit->viewDistance + CHUNK_SIZE)
	if (fabsf(curUnit->x-x)>dist||fabsf(curUnit->y-y)>dist||fabsf(curUnit->z-z)>dist) { // die if too far away from unit
		life = -1;
		return;
	}
#undef dist
	life -= dt;
	float dt_f = dt/1000.0f;
	switch (type) {
		case FALLING_LEAF:
			if (param1 >= 0) {
				float sw = sinf(life/250.0f)*2;
				x += param1*sw*dt_f;
				z += param2*sw*dt_f;
			}
			
			y -= 2.0f*dt_f;
			int newParam3 = int(x) + WORLD_X*int(y-(size/2)) + WORLD_X*WORLD_Y*int(z);
			if (param3 != newParam3) {
				param3 = newParam3;
				if (Block::block(world.getBlock(x, y-(size/2), z))->isSolid) {
					y = roundf(y)+(size/2);
					param1 = param3 = -1;
					if (Block::block(world.getBlock(x, y, z))->isSolid) {
						// fell into the side of a block
						life = 0;
					}
				}
			}
			break;
	}
}