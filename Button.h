//
//  Button.h
//  Bedrock
//
//  Created by Quinn on 1/18/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include <string>

#include "HUDElement.h"
#include "ortho.h"



class Button: public HUDElement {
public:
	typedef void (*callbackFunc_ptr)(int);
	
	Button(std::string text, callbackFunc_ptr oncl, int arg, int x, int y, int w, int h, ortho::color bgc, ortho::color hovc, ortho::color txtc):
				HUDElement(x,y,w,h), text(text), onClick(oncl),onClickArg(arg), color(bgc),hoverColor(hovc),textColor(txtc) {}
	virtual ~Button() {}
	
	bool draw(int mouseX, int mouseY);
	
	callbackFunc_ptr onClick;
	int	onClickArg;
	
	std::string text;
	ortho::color color, hoverColor, textColor;
};