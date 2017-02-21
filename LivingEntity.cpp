//
//  LivingEntity.cpp
//  Bedrock
//
//  Created by Quinn on 9/27/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "LivingEntity.h"

void LivingEntity::damage(float dmg) {
	health -= dmg;
	boundHealth();
}

void LivingEntity::boundHealth() {
	if (health > maxHealth) health = maxHealth;
	else if (health <= 0) {
		health = 0;
		onDeath();
		remove = true;
	}
}