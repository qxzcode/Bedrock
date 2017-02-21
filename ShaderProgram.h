//
//  ShaderProgram.h
//  Bedrock
//
//  Created by Quinn on 11/2/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "OpenGL.h"
#include <string>

class ShaderProgram {
public:
	void newProgram(std::string vertSrc, std::string fragSrc);
	void deleteProgram();
	void useProgram();
	bool isCompiled() { return compiled; }
	GLuint progObj;
	
protected:
	bool compiled = false;
};