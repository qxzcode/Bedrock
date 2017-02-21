//
//  ChunkSlice.h
//  Bedrock
//
//  Created by Quinn on 1/5/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

class Chunk; class ChunkSlice;
#include "Chunk.h" // includes World.h and Block.h
#include "DisplayList.h"

#include "OpenGL.h" // GLfloat and drawing functions

// orientations
#define ORIENT_Xp 1
#define ORIENT_Xn 2
#define ORIENT_Yp 3
#define ORIENT_Yn 4
#define ORIENT_Zp 5
#define ORIENT_Zn 6

class ChunkSlice {
public:
	ChunkSlice(int p, int o):pos(p),orientation(o) {}
	void draw(Chunk* chunk, World* world);
	void markDirty() { dirty = true; }
	
protected:
	const int pos;		// local chunk coord of the plane
	const int orientation;	// orientation of the slice (see #define's above)
	
	GLfloat texCoords[CHUNK_SIZE*CHUNK_SIZE*4];
	DisplayList dl;
	bool dirty = true;
	bool empty = false;
};