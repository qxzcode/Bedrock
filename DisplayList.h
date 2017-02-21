//
//  DisplayList.h
//  Bedrock
//
//  Created by Quinn on 9/14/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once
#include "OpenGL.h"

class DisplayList {
public:
	void newList(bool execute = false);
	void endList();
	bool isCompiled() {return compiled;}
	void call();
	void deleteList();
protected:
	GLuint handle;
	bool compiled = false;
};