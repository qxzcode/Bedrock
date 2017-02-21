//
//  MineCommand.h
//  Bedrock
//
//  Created by Quinn on 1/19/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Command.h"

#include "util.h"

class MineCommand: public Command {
public:
	MineCommand(util::vec3i c1, util::vec3i c2);
	virtual ~MineCommand() {}
	
	virtual bool doCommand(Unit * unit, float dt_f);
	
	util::vec3i cMin, cMax;
	
protected:
	float timer = 0.6f;
	util::vec3i mining;
};