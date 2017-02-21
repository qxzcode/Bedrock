//
//  Rock.cpp
//  Bedrock
//
//  Created by Quinn on 4/6/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "Rock.h"

#include "textures.h"
#include "drawingUtil.h"
#include "util.h"

#define FACE_TC(f) {f/6.0,0},{(f+1)/6.0,0},{(f+1)/6.0,1},{f/6.0,1}
static const util::vec2f rockTexCoords[4*6] = {FACE_TC(0),FACE_TC(1),FACE_TC(2),FACE_TC(3),FACE_TC(4),FACE_TC(5)};

Rock::Rock():Item("Rock", {0.125,0.125,0.125}) {
//	static const int flipped[4*6] = {1,0,3,2};
//	static const int rotated[4*6] = {1,2,3,0};
//	
//	bool flip = random()%2;
//	int rotations = random()%4;
	for (int i = 0; i < 4*6; i++) {
		int in = i;
//		if (flip)
//			in = flipped[in];
//		for (int r = 0; r < rotations; r++)
//			in = rotated[in];
		texCoords[(i*2)+0] = rockTexCoords[in].x;
		texCoords[(i*2)+1] = rockTexCoords[in].y;
	}
}

void Rock::draw(float x, float y, float z, int state) {
	glBindTexture(GL_TEXTURE_2D, textures::items::rock);
	float d = state==DROPPED? 0 : 0.15;
	drawingUtil::drawBox(texCoords, EXPAND_VEC3(collisionSize*2), x+(state==HELD_LEFT?-0.1:(state==HELD_RIGHT?0.1:0)),y+d,z+d);
}