//
//  LivingEntity.h
//  Bedrock
//
//  Created by Quinn on 9/27/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Entity.h"

class LivingEntity: public Entity {
public:
	LivingEntity(float x, float y, float z, World * w, float maxH):Entity(x,y,z,w),maxHealth(maxH),health(maxH) {}
	LivingEntity(float x, float y, float z, World * w, float rx, float ry, float rz, float maxH):Entity(x,y,z,w,rx,ry,rz),maxHealth(maxH),health(maxH) {}
	EntityType type(){return EntityType::LIVING;}
	
	virtual void damage(float dmg);
	virtual void onDeath() {}
	int getDisplayHealth() {
		if (health < 1.0 && health > 0.0)
			return 1; // avoid displaying "0" when still barely alive
		return int(health);
	}
	
	float maxHealth, health;
	
protected:
	void boundHealth();
	std::string healthStr(){return std::string(" (")+getDisplayHealth()+"/"+int(maxHealth)+")";}
};