//
//  Wood.h
//  Bedrock
//
//  Created by Quinn on 3/13/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Block.h"

class Wood: public Block {
public:
	Wood(std::string name, blockID id, texture sides, texture top):Block(name, id, sides, true, false, true) {
		// set different top/bottom texture
		float texXn = (1.0f/BLOCK_SHEET_WIDTH) * float(top % BLOCK_SHEET_WIDTH),
			  texYn = (1.0f/BLOCK_SHEET_WIDTH) * float(top / BLOCK_SHEET_WIDTH),
			  texXp = texXn + (1.0f/BLOCK_SHEET_WIDTH),
			  texYp = texYn + (1.0f/BLOCK_SHEET_WIDTH);
		texCoords[2].c1x = texXn;
		texCoords[2].c1y = texYp;
		texCoords[2].c2x = texXp;
		texCoords[2].c2y = texYp;
		texCoords[2].c3x = texXp;
		texCoords[2].c3y = texYn;
		texCoords[2].c4x = texXn;
		texCoords[2].c4y = texYn;
		texCoords[3] = texCoords[2];
	}
	
	blockTexCoords& getTexCoords(blockTexCoords& tc, int x, int y, int z, World* w) {
		if (!randTex)
			return texCoords;
		
		//// randomly flip texture (so it looks like less of a grid) ////
		tc = texCoords;
		
		srandom(x + (WORLD_X*y) + (WORLD_X*WORLD_Y*z));
		// top/bottom
		for (int side = 2; side <= 3; side++) {
			switch (random() % 4) {
				case 0:
					tc[side].c1x = texCoords[side].c1x;
					tc[side].c1y = texCoords[side].c1y;
					tc[side].c2x = texCoords[side].c2x;
					tc[side].c2y = texCoords[side].c2y;
					tc[side].c3x = texCoords[side].c3x;
					tc[side].c3y = texCoords[side].c3y;
					tc[side].c4x = texCoords[side].c4x;
					tc[side].c4y = texCoords[side].c4y;
					break;
				case 1:
					tc[side].c1x = texCoords[side].c2x;
					tc[side].c1y = texCoords[side].c2y;
					tc[side].c2x = texCoords[side].c3x;
					tc[side].c2y = texCoords[side].c3y;
					tc[side].c3x = texCoords[side].c4x;
					tc[side].c3y = texCoords[side].c4y;
					tc[side].c4x = texCoords[side].c1x;
					tc[side].c4y = texCoords[side].c1y;
					break;
				case 2:
					tc[side].c1x = texCoords[side].c3x;
					tc[side].c1y = texCoords[side].c3y;
					tc[side].c2x = texCoords[side].c4x;
					tc[side].c2y = texCoords[side].c4y;
					tc[side].c3x = texCoords[side].c1x;
					tc[side].c3y = texCoords[side].c1y;
					tc[side].c4x = texCoords[side].c2x;
					tc[side].c4y = texCoords[side].c2y;
					break;
				case 3:
					tc[side].c1x = texCoords[side].c4x;
					tc[side].c1y = texCoords[side].c4y;
					tc[side].c2x = texCoords[side].c1x;
					tc[side].c2y = texCoords[side].c1y;
					tc[side].c3x = texCoords[side].c2x;
					tc[side].c3y = texCoords[side].c2y;
					tc[side].c4x = texCoords[side].c3x;
					tc[side].c4y = texCoords[side].c3y;
					break;
			}
		}
		// sides
		for (int side = 0; side < 6; side++) {
			if (side==2||side==3) continue;
			if (random() % 2) {
				tc[side].c1x = texCoords[side].c2x;
				tc[side].c2x = texCoords[side].c1x;
				tc[side].c3x = texCoords[side].c4x;
				tc[side].c4x = texCoords[side].c3x;
			}
			if (random() % 2) {
				tc[side].c1x = texCoords[side].c4x;
				tc[side].c2x = texCoords[side].c3x;
				tc[side].c3x = texCoords[side].c2x;
				tc[side].c4x = texCoords[side].c1x;
			}
		}
		
		return tc;
	}
};