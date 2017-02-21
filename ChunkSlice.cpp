//
//  ChunkSlice.cpp
//  Bedrock
//
//  Created by Quinn on 1/5/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "ChunkSlice.h"

#include "shaders.h"
#include "textures.h"
#include "globals.h"
#include "util.h"

void ChunkSlice::draw(Chunk* chunk, World* world) {
	if (dirty) {
		//// update texCoords
		dirty = false;
		empty = true;
		int blockPos = pos;
		blockTexCoords texCoordsTmp;
		sideTexCoords tc;
		blockID bid;
		int cwx = chunk->cx*CHUNK_SIZE,
			cwy = chunk->cy*CHUNK_SIZE,
			cwz = chunk->cz*CHUNK_SIZE;
		switch (orientation) {
#define XP_FACE(_x,_y,_z) Block::block(world->getBlock(wx+1,	wy,		wz)  )->isTransparent
#define XN_FACE(_x,_y,_z) Block::block(world->getBlock(wx-1,	wy,		wz)  )->isTransparent
#define YP_FACE(_x,_y,_z) Block::block(world->getBlock(wx,		wy+1,	wz)  )->isTransparent
#define YN_FACE(_x,_y,_z) Block::block(world->getBlock(wx,		wy-1,	wz)  )->isTransparent
#define ZP_FACE(_x,_y,_z) Block::block(world->getBlock(wx,		wy,		wz+1))->isTransparent
#define ZN_FACE(_x,_y,_z) Block::block(world->getBlock(wx,		wy,		wz-1))->isTransparent
#define ASSIGN_TEXCOORDS(_x,_y,_z, faceFunc) \
				for (int v1 = 0; v1 < CHUNK_SIZE; v1++)\
					for (int v2 = 0; v2 < CHUNK_SIZE; v2++) {\
						int index = (v1+(v2*CHUNK_SIZE)) * 4;\
						int wx = cwx+_x, wy = cwy+_y, wz = cwz+_z;\
						if ( (bid = chunk->getBlock(_x,_y,_z))&&bid!=GRASS_ID && faceFunc(_x,_y,_z)) {\
							tc = Block::block(bid)->getTexCoords(texCoordsTmp, wx,wy,wz, world)[orientation];\
							texCoords[ index+0 ] = tc.c1x;\
							texCoords[ index+1 ] = tc.c3x;\
							texCoords[ index+2 ] = tc.c1y;\
							texCoords[ index+3 ] = tc.c3y;\
							empty = false;\
						} else {\
							texCoords[ index+0 ] = -1;\
							texCoords[ index+1 ] = -1;\
							texCoords[ index+2 ] = -1;\
							texCoords[ index+3 ] = -1;\
						}\
					}
				
			case ORIENT_Xn:
				blockPos--;
				ASSIGN_TEXCOORDS(blockPos, v1, v2, XN_FACE);
				break;
			case ORIENT_Xp:
				ASSIGN_TEXCOORDS(blockPos, v1, v2, XP_FACE);
				break;
				
			case ORIENT_Yn:
				blockPos--;
				ASSIGN_TEXCOORDS(v1, blockPos, v2, YN_FACE);
				break;
			case ORIENT_Yp:
				ASSIGN_TEXCOORDS(v1, blockPos, v2, YP_FACE);
				break;
				
			case ORIENT_Zn:
				blockPos--;
				ASSIGN_TEXCOORDS(v1, v2, blockPos, ZN_FACE);
				break;
			case ORIENT_Zp:
				ASSIGN_TEXCOORDS(v1, v2, blockPos, ZP_FACE);
				break;
				
#undef ASSIGN_TEXCOORDS
#undef XP_FACE
#undef XN_FACE
#undef YP_FACE
#undef YN_FACE
#undef ZP_FACE
#undef ZN_FACE
		}
		
		if (empty) return;
		
		
		//// redraw quad into display list
		// (even though the geometry hasn't
		// changed, the texCoords must be updated)
		dl.newList(true); // execute = true
		
		// set uniform texCoords (an array of vec4's)
		glUniform4fv(shaders::fog_texCoordsLoc, CHUNK_SIZE*CHUNK_SIZE, texCoords);
		
		// draw quad.
		// values from glTexCoord are used to select the
		// final texture coords from the uniform array.
		glBegin(POLYGON_TYPE);
		{
			switch (orientation) {
				case ORIENT_Xp:
					glTexCoord2f(CHUNK_SIZE, 0);
					glVertex3f(pos+1, CHUNK_SIZE, 0);
					glTexCoord2f(CHUNK_SIZE, CHUNK_SIZE);
					glVertex3f(pos+1, CHUNK_SIZE, CHUNK_SIZE);
					glTexCoord2f(0, CHUNK_SIZE);
					glVertex3f(pos+1, 0, CHUNK_SIZE);
					glTexCoord2f(0, 0);
					glVertex3f(pos+1, 0, 0);
					break;
					
				case ORIENT_Xn:
					glTexCoord2f(0, 0);
					glVertex3f(pos-1, 0, 0);
					glTexCoord2f(0, CHUNK_SIZE);
					glVertex3f(pos-1, 0, CHUNK_SIZE);
					glTexCoord2f(CHUNK_SIZE, CHUNK_SIZE);
					glVertex3f(pos-1, CHUNK_SIZE, CHUNK_SIZE);
					glTexCoord2f(CHUNK_SIZE, 0);
					glVertex3f(pos-1, CHUNK_SIZE, 0);
					break;
			}
		}
		glEnd();
		
		// end display list
		dl.endList();
		
	} else if (!empty) dl.call();
}


