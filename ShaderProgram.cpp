//
//  ShaderProgram.cpp
//  Bedrock
//
//  Created by Quinn on 11/2/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "ShaderProgram.h"

#include "shaders.h"

#include <vector>

void ShaderProgram::newProgram(std::string vertSrc, std::string fragSrc) {
	// delete any existing program
	deleteProgram();
	
	
	// create shader handles
	GLuint	vertObj = glCreateShader(GL_VERTEX_SHADER),
	fragObj = glCreateShader(GL_FRAGMENT_SHADER);
	
	// set source code
	const GLchar* vSrc = vertSrc.c_str();
	const GLchar* fSrc = fragSrc.c_str();
	glShaderSource(vertObj, 1, (const GLchar* /*const*/ *)&vSrc, NULL);
	glShaderSource(fragObj, 1, (const GLchar* /*const*/ *)&fSrc, NULL);
	
	// compile shaders
	glCompileShader(vertObj);
	glCompileShader(fragObj);
	
	// check for compile errors
	GLint success = 0;
	glGetShaderiv(vertObj, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		printf("ERROR COMPILING VERTEX SHADER:\n\n");
		GLint maxLength = 0;
		glGetShaderiv(vertObj, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(vertObj, maxLength, &maxLength, &errorLog[0]);
		printf("%s\n", &errorLog[0]);
		
		// don't leak shaders
		glDeleteShader(vertObj);
		glDeleteShader(fragObj);
		return;
	}
	glGetShaderiv(fragObj, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		printf("ERROR COMPILING FRAGMENT SHADER\n");
		GLint maxLength = 0;
		glGetShaderiv(fragObj, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(fragObj, maxLength, &maxLength, &errorLog[0]);
		printf("%s\n", &errorLog[0]);
		
		// don't leak shaders
		glDeleteShader(vertObj);
		glDeleteShader(fragObj);
		return;
	}
	
	
	// create program
	progObj = glCreateProgram();
	
	// attach shaders
	glAttachShader(progObj, vertObj);
	glAttachShader(progObj, fragObj);
	
	// link program
	glLinkProgram(progObj);
	
	// check for linking errors
	glGetProgramiv(progObj, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		printf("ERROR LINKING PROGRAM\n");
		GLint maxLength = 0;
		glGetProgramiv(progObj, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(progObj, maxLength, &maxLength, &errorLog[0]);
		printf("%s\n", &errorLog[0]);
		
		// don't leak program or shaders
		deleteProgram();
		glDeleteShader(vertObj);
		glDeleteShader(fragObj);
		return;
	}
	
	// clean up
	glDetachShader(progObj, vertObj);
	glDetachShader(progObj, fragObj);
	glDeleteShader(vertObj);
	glDeleteShader(fragObj);
	
	compiled = true; // yay!
}

void ShaderProgram::deleteProgram() {
	glDeleteProgram(progObj);
	progObj = 0;
	compiled = false;
}

void ShaderProgram::useProgram() {
	glUseProgram(progObj);
	shaders::curProg = this;
}