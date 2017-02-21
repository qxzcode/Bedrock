//
//  OcclusionChunk.h
//  Bedrock
//
//  Created by Quinn on 3/23/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#pragma once

#define OCCLUSION_RESOLUTION 4
#define OCCLUSION_CELL_SIZE (CHUNK_SIZE/OCCLUSION_RESOLUTION)
#if CHUNK_SIZE%OCCLUSION_RESOLUTION != 0
#error OCCLUSION_RESOLUTION must divide CHUNK_SIZE evenly!
#endif

typedef unsigned char occVal;
#define OCC_NONE	0 // no renderable blocks
#define OCC_PARTIAL	1 // at least one renderable block
#define OCC_FULL	2 // completely opaque


class OcclusionChunk;
#include "Chunk.h"
class Chunk;

class OcclusionChunk {
public:
	OcclusionChunk(Chunk * chunk);
	
	occVal getValue(int cx, int cy, int cz) {return data[cx][cy][cz];}
	void updateCell(int cx, int cy, int cz);
	
protected:
	occVal data[OCCLUSION_RESOLUTION][OCCLUSION_RESOLUTION][OCCLUSION_RESOLUTION];
	const Chunk * const chunk;
};