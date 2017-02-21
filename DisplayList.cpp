//
//  DisplayList.cpp
//  Bedrock
//
//  Created by Quinn on 9/14/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include <iostream>
#include "DisplayList.h"

void DisplayList::newList(bool execute) {
	deleteList();
	handle = glGenLists(1);
	glNewList(handle, execute? GL_COMPILE_AND_EXECUTE : GL_COMPILE);
}

void DisplayList::endList() {
	glEndList();
	compiled = true;
}

void DisplayList::call() {
	if (compiled)
		glCallList(handle);
	else std::cerr << "Tried to call a non-compiled DisplayList\n";
}

void DisplayList::deleteList() {
	if (compiled) {
		glDeleteLists(handle, 1);
		handle = 0;
		compiled = false;
	}
}