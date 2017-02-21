//
//  MoveCommand.h
//  Bedrock
//
//  Created by Quinn on 1/6/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Command.h"

#include "util.h"

class MoveCommand: public Command {
public:
	MoveCommand(util::vec3f t):to(t) {}
	virtual ~MoveCommand() {}
	
	virtual bool doCommand(Unit * unit, float dt_f);
	
	util::vec3f to;
};