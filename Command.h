//
//  Command.h
//  Bedrock
//
//  Created by Quinn on 1/6/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

class Command;
#include "Unit.h"

class Command {
public:
	virtual ~Command() {}
	
	// returns true if the command is complete
	virtual bool doCommand(Unit * unit, float dt_f) = 0;
};