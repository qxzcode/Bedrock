//
//  ortho.h
//  Bedrock
//
//  Created by Quinn on 12/22/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include <string>
#include <sstream>
#include "OpenGL.h"


// text centering
#include "textures.h"
#define TEXTWIDTH(str,size) (str.length()*size*7*FONT_SHEET_RATIO)
#define CENTER_TEXT(str,size,xCent) ((xCent)-(TEXTWIDTH(str, size)/2))
#define CENTER_TEXT_WIN(str,size) CENTER_TEXT(str,size,(width/2))
#define VERT_CENTER_TEXT(size,yCent) (yCent-(size*7/2))


namespace ortho {
	struct color {
		float r, g, b, a;
	};
	const color white	= {1,1,1, 1},
				black	= {0,0,0, 1},
				red		= {1,0,0, 1},
				orange	= {1,.5,0, 1},
				green	= {0,1,0, 1},
				blue	= {0,0,1, 1},
				purple	= {.5,0,1, 1};
	
	
	void orthoOn();
	void orthoOff();
	
	void drawImage(GLuint img, int x, int y, int width, int height);
	void drawRect(color col, int x, int y, int width, int height);
	void drawString(std::string str, int x, int y, int size, color col);
	inline void drawText(std::string str, int x, int y, int size) {
		drawString(str, x+1, y+1, size, white);
		drawString(str, x, y, size, black);
	}
	template <typename T> inline std::string toString(T val) {
		return (std::ostringstream()<<val).str();
	}
	
	void drawLoadingScreen(std::string title, std::string status);
}