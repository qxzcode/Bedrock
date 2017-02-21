//
//  Button.cpp
//  Bedrock
//
//  Created by Quinn on 1/18/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "Button.h"

#include "input.h"

bool Button::draw(int mouseX, int mouseY) {
	bool inButton = mouseX>=xPos && mouseX<=xPos+width && mouseY>=yPos && mouseY<=yPos+height;
	
	// background rect
	ortho::drawRect(inButton?
					hoverColor : color, xPos, yPos, width, height);
	
	// text
	ortho::drawString(text, CENTER_TEXT(text, 2, xPos+(width/2)), VERT_CENTER_TEXT(2, yPos+(height/2)), 2, textColor);
	
	// handle clicks
	if (inButton && input::mouseLeftClicked) {
		input::mouseLeftClicked = false;
		onClick(onClickArg);
	}
	
	return inButton;
}