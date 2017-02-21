//
//  Menu.h
//  Bedrock
//
//  Created by Quinn on 1/18/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include <vector>

#include "ortho.h"
#include "HUDElement.h"

class Menu: public HUDElement {
public:
	Menu(int x, int y, int w, int h, ortho::color bgc):
				HUDElement(x,y,w,h), color(bgc) {}
	virtual ~Menu() { clearItems(); }
	
	bool draw(int mouseX, int mouseY);
	void addItem(HUDElement* item_ptr) {
		items.push_back(item_ptr);
	}
	void clearItems() {
		for (HUDElement* item_ptr : items)
			delete item_ptr;
		items.clear();
	}
	
	ortho::color color;
	bool isOpen = false;
	
protected:
	std::vector<HUDElement*> items;
};