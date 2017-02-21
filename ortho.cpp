//
//  ortho.cpp
//  Bedrock
//
//  Created by Quinn on 12/22/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "ortho.h"

#include "util.h"
#include "globals.h"
#include "shaders.h"

#include "OpenGL.h"

void ortho::drawLoadingScreen(std::string title, std::string status) {
	// draw "loading screen"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the screen
	
	orthoOn();
	{
		drawText(title, CENTER_TEXT_WIN(title, 3), 150, 3);
		drawText(status, CENTER_TEXT_WIN(status, 2), 225, 2);
	}
	orthoOff();
	
	util::swapBuffers();
}

void ortho::orthoOn() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	shaders::ortho.useProgram();
	glUseProgram(0);
}

void ortho::orthoOff() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	shaders::fog.useProgram();
}

void ortho::drawImage(GLuint img, int x, int y, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, img);
	glBegin(GL_TRIANGLE_FAN);
	{
		glColor3f(1, 1, 1);
		
		glTexCoord2f(0, 1);
		glVertex2i(x, y+height);
		glTexCoord2f(0, 0);
		glVertex2i(x, y);
		glTexCoord2f(1, 0);
		glVertex2i(x+width, y);
		glTexCoord2f(1, 1);
		glVertex2i(x+width, y+height);
	}
	glEnd();
}

void ortho::drawRect(color col, int x, int y, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_TRIANGLE_FAN);
	{
		glColor4f(col.r, col.g, col.b, col.a);
		
		glVertex2i(x, y+height);
		glVertex2i(x, y);
		glVertex2i(x+width, y);
		glVertex2i(x+width, y+height);
	}
	glEnd();
}

#define NUM_CHARS 44
#define CHAR_TEX_WIDTH (1.0f/NUM_CHARS)

void ortho::drawString(std::string str, int x, int y, int size, color col) {
	size *= 7;
	int charWidth = size * FONT_SHEET_RATIO;
	glBindTexture(GL_TEXTURE_2D, textures::hud::font);
	glColor3f(col.r, col.g, col.b);
	
	for (int i = 0, c = str[i]; i < str.length(); c = str[++i]) {
		if (c == 32) continue; // space
		c = toupper(c); // A = 0, Z = 25
		
		// convert char code to font index
		int ci;
		if (c >= 65 && c < 65+26) { // letter
			ci = c - 65 + 10;
		} else if (c >= 48 && c < 48+10) { // digit
			ci = c - 48;
		} else if (c == 46) { // period
			ci = 10+26 + 0;
		} else if (c == 63) { // question mark
			ci = 10+26 + 1;
		} else if (c == 33) { // exclamation point
			ci = 10+26 + 2;
		} else if (c == 58) { // colon
			ci = 10+26 + 3;
		} else if (c == 44) { // comma
			ci = 10+26 + 4;
		} else if (c == 47) { // forward slash
			ci = 10+26 + 5;
		} else if (c == 40 || c == 41) { // parenthesis
			ci = 10+26 + 6 + c-40;
		} else {
			printf("INVALID CHARACTER: %c\nIN STRING: \"%s\"\n", str[i], str.c_str());
			return;
		}
		
		float texX = CHAR_TEX_WIDTH * (ci);
		int cx = (i * charWidth) + x;
		glBegin(GL_TRIANGLE_FAN);
		{
			
			glTexCoord2f(texX, 1);
			glVertex2i(cx, y+size);
			glTexCoord2f(texX, 0);
			glVertex2i(cx, y);
			glTexCoord2f(texX+CHAR_TEX_WIDTH, 0);
			glVertex2i(cx+charWidth, y);
			glTexCoord2f(texX+CHAR_TEX_WIDTH, 1);
			glVertex2i(cx+charWidth, y+size);
		}
		glEnd();
	}
}



