//
//  drawingUtil.cpp
//  Bedrock
//
//  Created by Quinn on 4/6/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "drawingUtil.h"

void drawingUtil::drawBox(const float* const texCoords, float dimX,float dimY,float dimZ, float offsetX,float offsetY,float offsetZ) {
#define TEXCOORDS(f,v)	texCoords[(f*4*2)+(v*2)+0],	texCoords[(f*4*2)+(v*2)+1]
	dimX/=2;dimY/=2;dimZ/=2;
	glBegin(GL_QUADS);
	{
		/* RIGHT (X+) */
		glColor3f(0.9, 0.9, 0.9);
		glTexCoord2f(TEXCOORDS(0,0));
		glVertex3f(dimX+offsetX, -dimY+offsetY, -dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(0,1));
		glVertex3f(dimX+offsetX, dimY+offsetY, -dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(0,2));
		glVertex3f(dimX+offsetX, dimY+offsetY, dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(0,3));
		glVertex3f(dimX+offsetX, -dimY+offsetY, dimZ+offsetZ);
		
		/* LEFT (X-) */
		glColor3f(0.9, 0.9, 0.9);
		glTexCoord2f(TEXCOORDS(1,0));
		glVertex3f(-dimX+offsetX, -dimY+offsetY, dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(1,1));
		glVertex3f(-dimX+offsetX, dimY+offsetY, dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(1,2));
		glVertex3f(-dimX+offsetX, dimY+offsetY, -dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(1,3));
		glVertex3f(-dimX+offsetX, -dimY+offsetY, -dimZ+offsetZ);
		
		/* TOP (Y+) */
		glColor3f(1, 1, 1);
		glTexCoord2f(TEXCOORDS(2,0));
		glVertex3f(-dimX+offsetX, dimY+offsetY, -dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(2,1));
		glVertex3f(-dimX+offsetX, dimY+offsetY, dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(2,2));
		glVertex3f(dimX+offsetX, dimY+offsetY, dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(2,3));
		glVertex3f(dimX+offsetX, dimY+offsetY, -dimZ+offsetZ);
		
		/* BOTTOM (Y-) */
		glColor3f(0.7, 0.7, 0.7);
		glTexCoord2f(TEXCOORDS(3,0));
		glVertex3f(dimX+offsetX, -dimY+offsetY, -dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(3,1));
		glVertex3f(dimX+offsetX, -dimY+offsetY, dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(3,2));
		glVertex3f(-dimX+offsetX, -dimY+offsetY, dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(3,3));
		glVertex3f(-dimX+offsetX, -dimY+offsetY, -dimZ+offsetZ);
		
		/* FRONT (Z+) */
		glColor3f(0.8, 0.8, 0.8);
		glTexCoord2f(TEXCOORDS(4,0));
		glVertex3f(dimX+offsetX, -dimY+offsetY, dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(4,1));
		glVertex3f(dimX+offsetX, dimY+offsetY, dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(4,2));
		glVertex3f(-dimX+offsetX, dimY+offsetY, dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(4,3));
		glVertex3f(-dimX+offsetX, -dimY+offsetY, dimZ+offsetZ);
		
		/* BACK (Z-) */
		glColor3f(0.8, 0.8, 0.8);
		glTexCoord2f(TEXCOORDS(5,0));
		glVertex3f(-dimX+offsetX, -dimY+offsetY, -dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(5,1));
		glVertex3f(-dimX+offsetX, dimY+offsetY, -dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(5,2));
		glVertex3f(dimX+offsetX, dimY+offsetY, -dimZ+offsetZ);
		glTexCoord2f(TEXCOORDS(5,3));
		glVertex3f(dimX+offsetX, -dimY+offsetY, -dimZ+offsetZ);
	}
	glEnd();
#undef TEXCOORDS
}
