//
//  Menu.cpp
//  Bedrock
//
//  Created by Quinn on 1/18/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "Menu.h"

bool Menu::draw(int mouseX, int mouseY) {
	if (isOpen) {
		// background rect
#define BORDER_WIDTH 10
		ortho::drawRect(color, xPos, yPos, width, height);
		ortho::drawRect(color, xPos, yPos, BORDER_WIDTH, height);
		ortho::drawRect(color, xPos+width-BORDER_WIDTH, yPos, BORDER_WIDTH, height);
		ortho::drawRect(color, xPos+BORDER_WIDTH, yPos, width-(BORDER_WIDTH*2), BORDER_WIDTH);
		ortho::drawRect(color, xPos+BORDER_WIDTH, yPos+height-BORDER_WIDTH, width-(BORDER_WIDTH*2), BORDER_WIDTH);
		
		// menu items
		bool inMenu = mouseX>=xPos && mouseX<=xPos+width && mouseY>=yPos && mouseY<=yPos+height;
#define PADDING 7
		int curY = yPos + BORDER_WIDTH;
		for (int i = 0; i < items.size(); i++) {
			HUDElement& item = *items[i];
			
			item.xPos = xPos + BORDER_WIDTH;
			item.yPos = curY;
			item.width = width - (BORDER_WIDTH*2);
			inMenu |= item.draw(mouseX, mouseY);
			
			curY += item.height;
			curY += PADDING;
		}
		
		return inMenu;
	} else {
		return false;
	}
}