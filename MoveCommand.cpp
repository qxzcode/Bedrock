//
//  MoveCommand.cpp
//  Bedrock
//
//  Created by Quinn on 1/6/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "MoveCommand.h"

#include "math.h"

bool MoveCommand::doCommand(Unit * unit, float dt_f) {
	unit->yaw = util::mod(unit->yaw, 360);
	if (unit->yaw < -180) unit->yaw += 360;
	if (unit->yaw > 180) unit->yaw -= 360;
	float targetYaw = atan2f(unit->x-to.x, to.z-unit->z)/DEG_TO_RAD;
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
	
	// turn towards destination and move forward
	if (fabsf(targetYaw - unitYaw) > 10)
		unit->yaw += util::sign(targetYaw - unitYaw)*100 * dt_f;
	unit->accelerate();
	
	// return true if within 0.5 blocks of destination
	return (to - util::vec3f{unit->x,to.y,unit->z}).length() < 0.5f;
}
