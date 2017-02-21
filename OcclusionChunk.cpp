//
//  OcclusionChunk.cpp
//  Bedrock
//
//  Created by Quinn on 3/23/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "OcclusionChunk.h"

OcclusionChunk::OcclusionChunk(Chunk * chunk):chunk(chunk) {
	for (int x = 0; x < OCCLUSION_RESOLUTION; x++)
		for (int y = 0; y < OCCLUSION_RESOLUTION; y++)
			for (int z = 0; z < OCCLUSION_RESOLUTION; z++)
				updateCell(x, y, z);
}

void OcclusionChunk::updateCell(int cx, int cy, int cz) {
	int xmin = cx * OCCLUSION_CELL_SIZE,
		xmax = xmin + OCCLUSION_CELL_SIZE-1,
		ymin = cy * OCCLUSION_CELL_SIZE,
		ymax = ymin + OCCLUSION_CELL_SIZE-1,
		zmin = cz * OCCLUSION_CELL_SIZE,
		zmax = zmin + OCCLUSION_CELL_SIZE-1;
	bool full = true, partial = false;
	
	for (int x = xmin; x<=xmax; x++) {
		for (int y = ymin; y<=ymax; y++) {
			for (int z = zmin; z<=zmax; z++) {
				bool outer = x==xmin||x==xmax||y==ymin||y==ymax||z==zmin||z==zmax;
				if (!outer && partial)
					continue;
				blockID block = chunk->getBlock(x, y, z);
				if (block!=AIR_ID&&block!=GRASS_ID) partial = true;
				if (Block::block(block)->isTransparent) full = false;
			}
		}
	}
	
	data[cx][cy][cz] = full? OCC_FULL :	partial? OCC_PARTIAL : OCC_NONE;
}