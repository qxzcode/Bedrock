//
//  MineCommand.cpp
//  Bedrock
//
//  Created by Quinn on 1/19/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "MineCommand.h"

MineCommand::MineCommand(util::vec3i c1, util::vec3i c2) {
#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)
	cMin = {min(c1.x,c2.x), min(c1.y,c2.y), min(c1.z,c2.z)};
	cMax = {max(c1.x,c2.x), max(c1.y,c2.y), max(c1.z,c2.z)};
	mining = c1;
#undef min
#undef max
}

static inline void getBlocksInSelection(util::vec3i cMin, util::vec3i cMax, std::vector<util::vec3i> & blocks) {
	for (int x = cMin.x; x <= cMax.x; x++)
		for (int y = cMin.y; y <= cMax.y; y++)
			for (int z = cMin.z; z <= cMax.z; z++) {
				if (Block::block(world.getBlock(x, y, z))->isSolid) {
					blocks.push_back({x, y, z});
				}
			}
}

bool MineCommand::doCommand(Unit * unit, float dt_f) {
	// turn towards block to mine
	unit->yaw = util::mod(unit->yaw, 360);
	if (unit->yaw < -180) unit->yaw += 360;
	if (unit->yaw > 180) unit->yaw -= 360;
	float targetYaw = atan2f(unit->x-mining.x, mining.z-unit->z)/DEG_TO_RAD;
	// find the closest coterminal angle
	float unitYaw;
	{
		float closestYawDist = 9001;
		for (int turns = -1; turns <= 1; turns++) {
			float curYaw = unit->yaw + (turns*360);
			float curDist = fabsf(targetYaw-curYaw);
			if (curDist < closestYawDist) {
				closestYawDist = curDist;
				unitYaw = curYaw;
			}
		}
	}
	// turn towards destination
	if (fabsf(targetYaw - unitYaw) > 10)
		unit->yaw += util::sign(targetYaw - unitYaw)*100 * dt_f;
	
	if ((timer += dt_f) > 1.0f) {
		timer -= 1.0f;
		
		const util::vec3f unitPos = {unit->x, unit->y, unit->z};
		
		bool clear;
		float toMineDist = MAXFLOAT; // distance from unit to mining
		
		// find closest solid block in selection
		{
			std::vector<util::vec3i> blocks;
			getBlocksInSelection(cMin, cMax, blocks);
			
			if (blocks.size() == 0)
				return true; // all blocks have been mined!
			
			float curDist;
			for (util::vec3i block : blocks) {
				if ((curDist = (unitPos - (util::vec3f(block)+0.5f)).length()) < toMineDist) {
					mining = block;
					toMineDist = curDist;
				}
			}
			
			clear = (blocks.size() == 1);
		}
		
		/*for (int x = cMin.x; x <= cMax.x; x++)
			for (int y = cMin.y; y <= cMax.y; y++)
				for (int z = cMin.z; z <= cMax.z; z++) {
					if (Block::block(world.getBlock(x, y, z))->isSolid) {
						if (!toMine) {
							toMine = new util::vec3i{x, y, z};
							mining = *toMine;
						} else clear = false;
					}
				}*/
		
		if (toMineDist < 1.5f) {
			// mine block
			world.setBlockWithNotify(mining.x, mining.y, mining.z, 0);
			return clear;
		} else {
			// pause timer
			timer += 1.0f - dt_f;
			
			// move to get within range
			unit->accelerate();
		}
	}
	
	return false;
}