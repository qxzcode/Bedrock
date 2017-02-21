//
//  World.h
//  Bedrock
//
//  Created by Quinn on 9/17/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include <set>
#include <map>

#include "Particle.h"
class World;
#include "Entity.h"
class Entity;
#include "TileEntity.h"
#include "Block.h"
#include "Chunk.h"
class Chunk;
#include "OcclusionChunk.h"
#include "hud.h"

#define WORLD_CX 32	// world X dimension in chunks
#define WORLD_CY 32	// world Y dimension in chunks
#define WORLD_CZ 32	// world Z dimension in chunks

#define WORLD_X (WORLD_CX*CHUNK_SIZE)	// world X dimension in blocks
#define WORLD_Y (WORLD_CY*CHUNK_SIZE)	// world Y dimension in blocks
#define WORLD_Z (WORLD_CZ*CHUNK_SIZE)	// world Z dimension in blocks

#define MAX_PARTICLES 20000
extern const float gravity;

typedef std::set<Chunk*> chunkSet;

class World {
public:
	World();
	~World();
	
	void update(long dt);
	void draw();
	Chunk * loadChunk(int cx, int cy, int cz);
	Chunk * getChunk(int cx, int cy, int cz) { return chunks[(cx+WORLD_CX)%WORLD_CX][(cy+WORLD_CY)%WORLD_CY][(cz+WORLD_CZ)%WORLD_CZ]; }
	blockID getBlock(int x, int y, int z);
	occVal getOcclusionValue(int ocx, int ocy, int ocz);
	void setBlock(int x, int y, int z, blockID b);
	void setBlockWithNotify(int x, int y, int z, blockID b);
	void triggerBlockUpdate(int x, int y, int z, int ux, int uy, int uz) {
		Block::block(getBlock(x, y, z))->onBlockUpdate(this, x, y, z, ux, uy, uz);
	}
	void triggerVisible(int x, int y, int z) {
		Block::block(getBlock(x, y, z))->onVisible(this, x, y, z);
	}
	void triggerInvisible(int x, int y, int z) {
		Block::block(getBlock(x, y, z))->onVisible(this, x, y, z);
	}
	void addEntity(Entity* e) { if(e)entities.insert(e); }
	void removeEntity(Entity* e) { if(e){e->onRemoved();entities.erase(e);} }
	bool addParticle(int type, float x, float y, float z, bool old = false);
	TileEntity* getTileEntity(int x, int y, int z);
	bool setTileEntity(TileEntity* te, int x, int y, int z);
	void deleteTileEntity(int x, int y, int z);
	
	
	void destruct();
	
	Chunk * chunks[WORLD_CX][WORLD_CY][WORLD_CZ];
	bool firstDraw = true;
	
protected:
	void updateEntities(long dt);
	void updateTileEntities(long dt);
	void updateParticles(long dt);
	void drawChunks(const util::vec3f unitPos, const util::vec3f playerPos);
	void drawEntities();
	void drawParticles();
	
	chunkSet visibleChunks;
	std::set<Entity*> entities;
	std::map<int, TileEntity*> tileEntities;
	Particle particles[MAX_PARTICLES];
	GLfloat particlePositions[MAX_PARTICLES*3];
	GLfloat particleColorsSizes[MAX_PARTICLES*4];
	int particleCount, particleDrawCount;
	int lastParticleSlot = 0;
	
	friend bool hud::draw(std::string selection, int fps);
	friend void doSelect();
};