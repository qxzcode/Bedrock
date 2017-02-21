//
//  Quadruped.h
//  Bedrock
//
//  Created by Quinn on 3/30/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "LivingEntity.h"
#include "util.h"

class Quadruped: public LivingEntity {
protected:
	Quadruped(float x, float y, float z, World * w, float rx, float ry, float rz, float maxH):LivingEntity(x,y,z,w,rx,ry,rz,maxH) {}
public:
	
	virtual void update(long dt);
	virtual void draw();
	
	//////////////
	// MOVEMENT //
	//////////////
	
	void accelerate() {
		moving = true;
		if (lastCResult.y!=-1)
			return;
		float sinYaw = sinf(-yaw * DEG_TO_RAD);
		float cosYaw = cosf(-yaw * DEG_TO_RAD);
		if (sinYaw>=0? xSpeed < sinYaw*moveSpeed : xSpeed > sinYaw*moveSpeed)
			xSpeed += sinYaw * acceleration;
		if (cosYaw>=0? zSpeed < cosYaw*moveSpeed : zSpeed > cosYaw*moveSpeed)
			zSpeed += cosYaw * acceleration;
	}
	void decelerate() {
		moving = true;
		if (lastCResult.y!=-1)
			return;
		float sinYaw = sinf(-yaw * DEG_TO_RAD);
		float cosYaw = cosf(-yaw * DEG_TO_RAD);
		if (sinYaw>=0? xSpeed > -sinYaw*moveSpeed : xSpeed < -sinYaw*moveSpeed)
			xSpeed -= sinYaw * acceleration;
		if (cosYaw>=0? zSpeed > -cosYaw*moveSpeed : zSpeed < -cosYaw*moveSpeed)
			zSpeed -= cosYaw * acceleration;
	}
	void jump() {
		if (lastCResult.y!=-1)
			return;
		if (ySpeed < 10) {
			ySpeed = 10;
		}
	}
	float getFSpeed() {
		using util::vec2f;
		vec2f a = {xSpeed, zSpeed};
		vec2f bn = {sinf(-yaw * DEG_TO_RAD), cosf(-yaw * DEG_TO_RAD)};
		return a.dot(bn);
	}
	
	float yaw = 0;
	
	float moveSpeed;
	float acceleration;
	
protected:
	virtual void drawBody();
	virtual void drawHead();
	virtual void drawArm(bool left);
	virtual void drawLeg(bool left);
	
	GLuint texture;
	const float* texCoords = NULL;
	util::vec3f bodyDim, headDim, armDim, legDim;
	util::vec3f bodyOffset, headOffset, armOffset, legOffset;
	long anim = 0;
	
	collisionResult lastCResult;
	bool climbing = false, moving = false;
	
};