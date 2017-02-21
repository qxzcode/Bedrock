//
//  Block.cpp
//  Bedrock
//
//  Created by Quinn on 9/21/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "Block.h"

#include "Air.h"
#include "Stone.h"
#include "Dirt.h"
#include "RockyDirt.h"
#include "Grass.h"
#include "TallGrass.h"
#include "FlowerGrass.h"
#include "MountainGrass.h"
#include "Clay.h"
#include "RockyClay.h"
#include "Wood.h"
#include "Leaves.h"
#include "Water.h"
#include "textures.h"
#include "World.h"

#include <iostream>

Block::Block(std::string name, blockID id, texture texture, bool solid, bool transparent, bool rTex):name(name),isSolid(solid),isTransparent(transparent),randTex(rTex) {
	if (blocks[id]) std::cerr << "Block ID " << id << " is already taken! Ignoring...\n";
	else if (id < 0 || id >= NUM_BLOCKS) std::cerr << "Block ID " << id << " is out of range! Ignoring...\n";
	else {
		blocks[id] = this;
	}
	
	float texXn = (1.0f/BLOCK_SHEET_WIDTH) * float(texture % BLOCK_SHEET_WIDTH),
		  texYn = (1.0f/BLOCK_SHEET_WIDTH) * float(texture / BLOCK_SHEET_WIDTH),
		  texXp = texXn + (1.0f/BLOCK_SHEET_WIDTH),
		  texYp = texYn + (1.0f/BLOCK_SHEET_WIDTH);
	texCoords[0].c1x = texXn;
	texCoords[0].c1y = texYp;
	texCoords[0].c2x = texXp;
	texCoords[0].c2y = texYp;
	texCoords[0].c3x = texXp;
	texCoords[0].c3y = texYn;
	texCoords[0].c4x = texXn;
	texCoords[0].c4y = texYn;
	texCoords[1] = texCoords[0];
	texCoords[2] = texCoords[0];
	texCoords[3] = texCoords[0];
	texCoords[4] = texCoords[0];
	texCoords[5] = texCoords[0];
}

Block * Block::blocks[NUM_BLOCKS];

blockTexCoords& Block::getTexCoords(blockTexCoords& tc, int x, int y, int z, World* w) {
	if (!randTex)
		return texCoords;
	
	//// randomly rotate texture (so it looks like less of a grid) ////
	tc = texCoords;
	
	srandom(x + (WORLD_X*y) + (WORLD_X*WORLD_Y*z));
	for (int side = 0; side < 6; side++)
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
	
	return tc;
}

void Block::initBlocks() {
	new Air();
	new Stone(STONE_ID);
	new Dirt(DIRT_ID);
	new RockyDirt(ROCKY_DIRT_ID);
	new Grass(GRASS_ID);
	new TallGrass(TALL_GRASS_ID);
	new FlowerGrass(FLOWER_GRASS_ID);
	new MountainGrass(MOUNTAIN_GRASS_ID);
	new Clay(CLAY_ID);
	new RockyClay(ROCKY_CLAY_ID);
	new Wood("Wood", WOOD_ID, 11, 12);
	new Leaves("Leaves", LEAVES_ID, 13);
	new Leaves("Leaves", LEAVES2_ID, 14);
	new Wood("Birch Wood", BIRCH_WOOD_ID, 19, 20);
	new Leaves("Birch Leaves", BIRCH_LEAVES_ID, 21);
	new Leaves("Birch Leaves", BIRCH_LEAVES2_ID, 22);
	new Water(WATER_ID);
}