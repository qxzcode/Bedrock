//
//  MenuButton.cpp
//  Bedrock
//
//  Created by Quinn on 1/17/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "MenuButton.h"

#include "textures.h"

bool MenuButton::draw(int mouseX, int mouseY) {
	bool inButton = mouseX>=xPos && mouseX<=xPos+width && mouseY>=yPos && mouseY<=yPos+height;
	if (inButton) menu.isOpen = true;
	
	// background rect
	ortho::drawRect(menu.isOpen?
					hoverColor : color, xPos, yPos, width, height);
	
	// text
	ortho::drawString(text, CENTER_TEXT(text, 2, xPos+(width/2)), VERT_CENTER_TEXT(2, yPos+(height/2)), 2, textColor);
	
	// menu arrow
	ortho::drawImage(textures::hud::menus, xPos+width-20, VERT_CENTER_TEXT(2, yPos+(height/2)), 5*2, 7*2);
	
	// menu
	bool inMenu = menu.draw(mouseX, mouseY);
	if (!(inButton || inMenu)) menu.isOpen = false;
	
	return inButton || inMenu;
}