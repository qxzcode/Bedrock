//
//  MenuButton.h
//  Bedrock
//
//  Created by Quinn on 1/17/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include <string>

#include "ortho.h"
#include "HUDElement.h"
#include "Menu.h"

class MenuButton: public HUDElement {
public:
	MenuButton(std::string text, int x, int y, int w, int h, ortho::color bgc, ortho::color hovc, ortho::color txtc, Menu& m):
					HUDElement(x,y,w,h), text(text), color(bgc),hoverColor(hovc),textColor(txtc), menu(m) {}
	virtual ~MenuButton() {}
	
	bool draw(int mouseX, int mouseY);
	
	std::string text;
	ortho::color color, hoverColor, textColor;
	Menu& menu;
};