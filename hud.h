//
//  hud.h
//  Bedrock
//
//  Created by Quinn on 1/16/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "DisplayList.h"
#include "MenuButton.h"
#include "util.h"

namespace hud {
	void buildMenus();
	bool draw(std::string selection, int fps);
	
	extern DisplayList hudBG;
	
	
	enum struct CommandEnum {
		// meta
		NONE = 0, CANCEL, 
		
		// generic
		MOVE,
		
		// librarian
		//...
		
		// miner
		MINE,
	};
	extern CommandEnum issuingCmd;
}