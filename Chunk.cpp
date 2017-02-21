//
//  Chunk.cpp
//  Bedrock
//
//  Created by Quinn on 9/17/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "Chunk.h"

#include "OpenGL.h"

#include "textures.h"

Chunk::Chunk(int cx, int cy, int cz, World * w):cx(cx),cy(cy),cz(cz),world(w) {
	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int y = 0; y < CHUNK_SIZE; y++)
			for (int z = 0; z < CHUNK_SIZE; z++)
				blocks[x][y][z] = 0;
}

Chunk::~Chunk() {
	//dl.deleteList();
}

void Chunk::setBlock(int x, int y, int z, blockID b) {
	if (blocks[x][y][z] != (blocks[x][y][z]=b)) {
		is_empty = false;
		needsRedraw = true;
		Chunk * rdc;
		
		if (occlusionChunk)
			occlusionChunk->updateCell(x/OCCLUSION_RESOLUTION, y/OCCLUSION_RESOLUTION, z/OCCLUSION_RESOLUTION);
		
		// six sides
		if (x==0 && (rdc=world->getChunk(cx-1, cy, cz))) rdc->needsRedraw = true;
		if (y==0 && (rdc=world->getChunk(cx, cy-1, cz))) rdc->needsRedraw = true;
		if (z==0 && (rdc=world->getChunk(cx, cy, cz-1))) rdc->needsRedraw = true;
		if (x==CHUNK_SIZE-1 && (rdc=world->getChunk(cx+1, cy, cz))) rdc->needsRedraw = true;
		if (y==CHUNK_SIZE-1 && (rdc=world->getChunk(cx, cy+1, cz))) rdc->needsRedraw = true;
		if (z==CHUNK_SIZE-1 && (rdc=world->getChunk(cx, cy, cz+1))) rdc->needsRedraw = true;
		
		// twelve edges (macros to the rescue!)
#define DO_EDGES(edgeFunc) \
		if (edgeFunc(-1, -1)) rdc->needsRedraw = true;\
		if (edgeFunc(+1, -1)) rdc->needsRedraw = true;\
		if (edgeFunc(+1, +1)) rdc->needsRedraw = true;\
		if (edgeFunc(-1, +1)) rdc->needsRedraw = true;
#define XY_EDGE(xVal, yVal) x==(xVal<0?0:CHUNK_SIZE-1) && y==(yVal<0?0:CHUNK_SIZE-1) && (rdc=world->getChunk(cx+xVal, cy+yVal, cz))
#define YZ_EDGE(yVal, zVal) y==(yVal<0?0:CHUNK_SIZE-1) && z==(zVal<0?0:CHUNK_SIZE-1) && (rdc=world->getChunk(cx, cy+yVal, cz+zVal))
#define XZ_EDGE(xVal, zVal) x==(xVal<0?0:CHUNK_SIZE-1) && z==(zVal<0?0:CHUNK_SIZE-1) && (rdc=world->getChunk(cx+xVal, cy, cz+zVal))
		DO_EDGES(XY_EDGE);
		DO_EDGES(YZ_EDGE);
		DO_EDGES(XZ_EDGE);
#undef XY_EDGE
#undef YZ_EDGE
#undef XZ_EDGE
#undef DO_EDGES
	}
}

void Chunk::onVisible() {//printf("CHUNK VISIBLE (%i, %i, %i)\n", cx, cy, cz);
	visible = true;
	// trigger blocks
	const int wcx = cx*CHUNK_SIZE, wcy = cy*CHUNK_SIZE, wcz = cz*CHUNK_SIZE;
	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int y = 0; y < CHUNK_SIZE; y++)
			for (int z = 0; z < CHUNK_SIZE; z++)
				world->triggerVisible(x+wcx, y+wcy, z+wcz);
	// generate occlusion data
	//occlusionChunk = new OcclusionChunk(this);
}

void Chunk::onInvisible() {//printf("CHUNK INVISIBLE (%i, %i, %i)\n", cx, cy, cz);
	visible = false;
	// trigger blocks
	const int wcx = cx*CHUNK_SIZE, wcy = cy*CHUNK_SIZE, wcz = cz*CHUNK_SIZE;
	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int y = 0; y < CHUNK_SIZE; y++)
			for (int z = 0; z < CHUNK_SIZE; z++)
				world->triggerInvisible(x+wcx, y+wcy, z+wcz);
	// free occlusion data
	delete occlusionChunk;
	// free geometry data
	/*xpFaces.clear();xnFaces.clear();ypFaces.clear();ynFaces.clear();zpFaces.clear();znFaces.clear();otherFaces.clear();
	needsRedraw = true;*/
}

void Chunk::toBytes(char* bytes) const {
	int y_mul;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			y_mul = (y*CHUNK_SIZE);
			for (int z = 0; z < CHUNK_SIZE; z++)
				bytes[x + y_mul + (z*CHUNK_SIZE*CHUNK_SIZE)] = getBlock(x, y, z);
		}
	}
}

void Chunk::fromBytes(char* bytes) {
	int y_mul;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			y_mul = (y*CHUNK_SIZE);
			for (int z = 0; z < CHUNK_SIZE; z++)
				setBlock(x, y, z, bytes[x + y_mul + (z*CHUNK_SIZE*CHUNK_SIZE)]);
		}
	}
}
