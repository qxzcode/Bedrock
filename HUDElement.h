//
//  HUDElement.h
//  Bedrock
//
//  Created by Quinn on 1/18/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

class HUDElement {
public:
	virtual ~HUDElement() {}
	
	int xPos, yPos, width, height;
	
	virtual bool draw(int mouseX, int mouseY) = 0;
	
protected:
	HUDElement(int x, int y, int w, int h):xPos(x),yPos(y),width(w),height(h) {}
};