//
//  Player.h
//  Bedrock
//
//  Created by Quinn on 9/29/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

class Player;
#include "globals.h"
#include "Unit.h"

#define PLAYER_SIZE 0.5, 0.5, 0.5

class Player: public Entity {
public:
	Player(float x, float y, float z, World * w);
	std::string name(){return"Player";}
	EntityType type(){return EntityType::PLAYER;}
	
	void update(long dt);
	void draw();
	void lookThrough();
	
	float pitch, yaw;
	float dbg_pitch, dbg_yaw;
	float dbg_x, dbg_y, dbg_z;
	float moveSpeed =
		#ifdef WORLD_VIEW
			60.0f/1000.0f;
		#else
			24.0f/1000.0f;
		#endif
	float lastTX, lastTY, lastTZ;
	
	void set1stPersonPos();
	
	bool controllingUnit = true;
	/// true for left, false for right
	bool activeArm = 0;
};