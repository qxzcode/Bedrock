//
//  input.h
//  Bedrock
//
//  Created by Quinn on 9/30/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include </Library/Frameworks/SDL.framework/Headers/SDL.h>

namespace input {
	
	void handleEvent(SDL_Event& event);
	
	extern bool playerForwards, playerBackwards, playerLeft, playerRight, playerDown, playerUp, dropItem, switchArms, quit, debug, fog;
	
	extern int mouseX, mouseY, mouseDragDX, mouseDragDY;
	extern bool mouseRightDown, mouseLeftClicked;
}