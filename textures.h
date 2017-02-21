//
//  textures.h
//  Bedrock
//
//  Created by Quinn on 9/20/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "OpenGL.h"


#define BLOCK_SHEET_WIDTH 8		// number of textures in each row/column of the blocks sprite sheet

#define UNIT_SHEET_WIDTH 36		// width of unit sheet in "pixels"
#define UNIT_SHEET_HEIGHT 15	// height of unit sheet in "pixels"

#define FONT_SHEET_RATIO (6.0f/7.0f) // character width / character height

namespace textures {
	void load(); // loads textures
	
	/// variables to store texture handles
	extern GLuint blocks, unit, bear;
	
	namespace items {
		extern GLuint rock;
	}
	
	namespace hud {
		extern GLuint font, menus;
	}
}