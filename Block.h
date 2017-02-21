//
//  Block.h
//  Bedrock
//
//  Created by Quinn on 9/21/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "textures.h"

typedef unsigned char	blockID;
typedef unsigned int	texture;

class World;

#include <string>

#define NUM_BLOCKS (WATER_ID+1)
enum BlockIDs {
	AIR_ID,
	STONE_ID,
	DIRT_ID, ROCKY_DIRT_ID,
	GRASS_ID, TALL_GRASS_ID, FLOWER_GRASS_ID, MOUNTAIN_GRASS_ID,
	CLAY_ID, ROCKY_CLAY_ID,
	WOOD_ID, LEAVES_ID, LEAVES2_ID,
	BIRCH_WOOD_ID, BIRCH_LEAVES_ID, BIRCH_LEAVES2_ID,
	WATER_ID
};

struct sideTexCoords {
	float c1x, c1y, c2x, c2y, c3x, c3y, c4x, c4y;
};

struct blockTexCoords {
	sideTexCoords sides[6]; // 0=X+, 1=X-, 2=Y+, 3=Y-, 4=Z+, 5=Z-
	
	inline sideTexCoords & operator[](int side) { return sides[side]; }
};

class Block {
public:
	virtual blockTexCoords& getTexCoords(blockTexCoords& tc, int x, int y, int z, World* w);
	virtual void onBlockUpdate(World* w, int x, int y, int z, int ux, int uy, int uz) {}
	virtual void onVisible(World* w, int x, int y, int z) {}
	virtual void onInvisible(World* w, int x, int y, int z) {}
	
	const bool isSolid, isTransparent;
	const std::string name;
	
	blockTexCoords texCoords;
	
	
	static void initBlocks();
	static inline Block * block(blockID id) { return blocks[id]; }
	
protected:
	Block(std::string name, blockID id, texture texture, bool solid, bool transparent, bool randTex = false);
	bool randTex;
	
	
	static Block * blocks[];
};

#include "World.h"
