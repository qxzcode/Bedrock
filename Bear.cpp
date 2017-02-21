//
//  Bear.cpp
//  Bedrock
//
//  Created by Quinn on 3/30/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "Bear.h"

const float bearTexCoords[192] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#define BODY_WIDTH	(1.0/2)
#define BODY_LENGTH (1.9/2)
#define HEAD_RADIUS	(0.7/2)
#define LEG_RADIUS	(0.4/2)
#define ARM_RADIUS	LEG_RADIUS

Bear::Bear(float x, float y, float z, World * w):Quadruped(x,y,z,w,1,BODY_WIDTH+(LEG_RADIUS*2),1,100) {
	moveSpeed = 2.0f;
	acceleration = 0.75f;
	
	texture = textures::bear;
	texCoords = bearTexCoords;
	bodyDim = util::vec3f{BODY_WIDTH,BODY_WIDTH,BODY_LENGTH}*2;
	headDim = util::vec3f{HEAD_RADIUS,HEAD_RADIUS,HEAD_RADIUS}*2;
	armDim = util::vec3f{ARM_RADIUS,ARM_RADIUS,ARM_RADIUS}*2;
	legDim = util::vec3f{LEG_RADIUS,LEG_RADIUS,LEG_RADIUS}*2;
	bodyOffset = {0,0,0};
	headOffset = {0,BODY_WIDTH,BODY_LENGTH+(HEAD_RADIUS/2)};
	armOffset = {0.3f,-BODY_WIDTH-ARM_RADIUS,0.6};
	legOffset = {0.3f,-BODY_WIDTH-LEG_RADIUS,-0.6};
}

void Bear::update(long dt) {
	float dt_f = dt/1000.0f;
	
	//// move... ////
	yaw += 45*dt_f;
	accelerate();
	// jump when running into a wall
	if ((lastCResult.x || lastCResult.z) && lastCResult.y==-1) {
		jump();
	}
	
	Quadruped::update(dt);
}

void Bear::draw() {
	Quadruped::draw();
}