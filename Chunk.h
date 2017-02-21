//
//  Chunk.h
//  Bedrock
//
//  Created by Quinn on 9/17/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "DisplayList.h"

#include "util.h"
#include "Block.h"
struct blockTexCoords; struct sideTexCoords;
#define CHUNK_SIZE 16
#define CHUNK_BYTES (CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE)
#include "OcclusionChunk.h"
#include "World.h"
class World;

#include <vector>
typedef std::vector<GLfloat> floatVector;

class Chunk {
public:
	Chunk(int cx, int cy, int cz, World * w);
	~Chunk();
	
	void draw(World* world, util::vec3f playerPos);
	blockID getBlock(int x, int y, int z) const { return blocks[x][y][z]; }
	void setBlock(int x, int y, int z, blockID b);
	void onVisible();
	void onInvisible();
	bool isEmpty() const { return is_empty; }
	int quadCount(util::vec3f playerPos) const {
		int sum = bufferSizes[6]/32;
		if (playerPos.x > cx*CHUNK_SIZE) sum += bufferSizes[0]/32;
		if (playerPos.x < cx*CHUNK_SIZE + CHUNK_SIZE) sum += bufferSizes[1]/32;
		if (playerPos.y > cy*CHUNK_SIZE) sum += bufferSizes[2]/32;
		if (playerPos.y > cy*CHUNK_SIZE + CHUNK_SIZE) sum += bufferSizes[3]/32;
		if (playerPos.z > cz*CHUNK_SIZE) sum += bufferSizes[4]/32;
		if (playerPos.z > cz*CHUNK_SIZE + CHUNK_SIZE) sum += bufferSizes[5]/32;
		return sum;
	}
	
	void toBytes(char* bytes) const;
	void fromBytes(char* bytes);
	
	const int cx, cy, cz;
	
	
	friend int main(int argc, char** argv);
	friend class World;
protected:
	blockID blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
	OcclusionChunk * occlusionChunk = NULL;
	World * world;
	
	// rendering stuff
	GLuint buffers[7] = {0,0,0,0,0,0,0};
	unsigned int bufferSizes[7];
	void drawBlock(World* world, util::vec3f wpos, blockTexCoords tc, util::vec3f pos, bool xp, bool xn, bool yp, bool yn, bool zp, bool zn);
	void drawTallGrass(sideTexCoords tc, util::vec3f pos);
	bool needsRedraw = true;
	bool is_empty = true;
	bool visible = false;
};
