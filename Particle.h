//
//  Particle.h
//  Bedrock
//
//  Created by Quinn on 3/13/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "util.h"

enum ParticleTypes {
	FALLING_LEAF
};

class Particle {
public:
	Particle():life(-1),type(-1) {}
	Particle(int type, float x, float y, float z, bool old = false);
	
	void update(long dt);
	
	int type;
	long life;
	util::vec3f color;
	float x, y, z, size;
	
	float param1, param2;
	int param3;
};