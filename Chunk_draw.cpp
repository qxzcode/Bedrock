//
//  Chunk_draw.c
//  Bedrock
//
//  Created by Quinn on 1/5/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "Chunk.h"

#include "shaders.h"
#include "globals.h"
#include "Grass.h"
#include "TallGrass.h"

// calculates ambient occlusion value for a vertex
static inline float vertexAO(bool side1, bool side2, bool corner) {
	int index;
	if (side1 && side2) {
		index = 1;
	} else
		index = 4 - (side1 + side2 + corner);
	
	return static_cast<float>(index)/6.0f + 0.25;
}

#define MAX_BUF_SIZE 8*CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE/2
static GLfloat tmp_buffers[7][MAX_BUF_SIZE];

#define PUSH_FLOAT(f) tmp_buffers[BUF_I][bufferSizes[BUF_I]++]=f;
#define TEXCOORDS(s, t) \
	{PUSH_FLOAT(s); PUSH_FLOAT(t);}
#define COLOR(br) \
	{PUSH_FLOAT(br); PUSH_FLOAT(0.0f); PUSH_FLOAT(0.0f);} // third value is ignored
#define GRASS_COLOR(br, val) \
	{PUSH_FLOAT(br); PUSH_FLOAT(val); PUSH_FLOAT(0.0f);} // third value is ignored
#define VERTEX(x, y, z) \
	{PUSH_FLOAT(x); PUSH_FLOAT(y); PUSH_FLOAT(z);}


void Chunk::drawBlock(World* world, util::vec3f wpos, blockTexCoords tc, util::vec3f pos, bool xp, bool xn, bool yp, bool yn, bool zp, bool zn) {
	if (!(xp||xn||yp||yn||zp||zn)) return;
	
	float light = 1, vLight;
	
#define CALC_AO_X(xd, yd, zd) \
	vLight = vertexAO(!Block::block(world->getBlock(wpos.x+xd, wpos.y+0 , wpos.z+zd))->isTransparent,\
					  !Block::block(world->getBlock(wpos.x+xd, wpos.y+yd, wpos.z+0 ))->isTransparent,\
					  !Block::block(world->getBlock(wpos.x+xd, wpos.y+yd, wpos.z+zd))->isTransparent);\
	COLOR(light*vLight);
	
#define CALC_AO_Y(xd, yd, zd) \
	vLight = vertexAO(!Block::block(world->getBlock(wpos.x+(0 ), wpos.y+yd, wpos.z+(zd)))->isTransparent,\
					  !Block::block(world->getBlock(wpos.x+(xd), wpos.y+yd, wpos.z+(0 )))->isTransparent,\
					  !Block::block(world->getBlock(wpos.x+(xd), wpos.y+yd, wpos.z+(zd)))->isTransparent);\
	COLOR(light*vLight);
	
#define CALC_AO_Z(xd, yd, zd) \
	vLight = vertexAO(!Block::block(world->getBlock(wpos.x+(0 ), wpos.y+(yd), wpos.z+zd))->isTransparent,\
					  !Block::block(world->getBlock(wpos.x+(xd), wpos.y+(0 ), wpos.z+zd))->isTransparent,\
					  !Block::block(world->getBlock(wpos.x+(xd), wpos.y+(yd), wpos.z+zd))->isTransparent);\
	COLOR(light*vLight);
	
#define lxp 1
#define lxn 1
#define lyp 1
#define lyn 1
#define lzp 1
#define lzn 1
	
#define LIGHT_TOP 1.00
#define LIGHT_BOTTOM 0.55 // Minecraft: 0.50
#define LIGHT_LR 0.85	  // Minecraft: 0.80
#define LIGHT_FB 0.7	  // Minecraft: 0.60
	
	if (xp) {
#define BUF_I 0
		// RIGHT (X+)
		light = lxp*LIGHT_LR;
		
		TEXCOORDS(tc[0].c1x, tc[0].c1y);
		CALC_AO_X(+1, -1, +1);
		VERTEX(pos.x+1, pos.y+0, pos.z+1);
		
		TEXCOORDS(tc[0].c2x, tc[0].c2y);
		CALC_AO_X(+1, -1, -1);
		VERTEX(pos.x+1, pos.y+0, pos.z+0);
		
		TEXCOORDS(tc[0].c3x, tc[0].c3y);
		CALC_AO_X(+1, +1, -1);
		VERTEX(pos.x+1, pos.y+1, pos.z+0);
		
		TEXCOORDS(tc[0].c4x, tc[0].c4y);
		CALC_AO_X(+1, +1, +1);
		VERTEX(pos.x+1, pos.y+1, pos.z+1);
#undef BUF_I
	}
	
	if (xn) {
#define BUF_I 1
		// LEFT (X-)
		light = lxn*LIGHT_LR;
		
		TEXCOORDS(tc[1].c1x, tc[1].c1y);
		CALC_AO_X(-1, -1, -1);
		VERTEX(pos.x+0, pos.y+0, pos.z+0);
		
		TEXCOORDS(tc[1].c2x, tc[1].c2y);
		CALC_AO_X(-1, -1, +1);
		VERTEX(pos.x+0, pos.y+0, pos.z+1);
		
		TEXCOORDS(tc[1].c3x, tc[1].c3y);
		CALC_AO_X(-1, +1, +1);
		VERTEX(pos.x+0, pos.y+1, pos.z+1);
		
		TEXCOORDS(tc[1].c4x, tc[1].c4y);
		CALC_AO_X(-1, +1, -1);
		VERTEX(pos.x+0, pos.y+1, pos.z+0);
#undef BUF_I
	}
	
	if (yp) {
#define BUF_I 2
		// TOP (Y+)
		light = lyp*LIGHT_TOP;
		
		TEXCOORDS(tc[2].c1x, tc[2].c1y);
		CALC_AO_Y(+1, +1, -1);
		VERTEX(pos.x+1, pos.y+1, pos.z+0);
		
		TEXCOORDS(tc[2].c2x, tc[2].c2y);
		CALC_AO_Y(-1, +1, -1);
		VERTEX(pos.x+0, pos.y+1, pos.z+0);
		
		TEXCOORDS(tc[2].c3x, tc[2].c3y);
		CALC_AO_Y(-1, +1, +1);
		VERTEX(pos.x+0, pos.y+1, pos.z+1);
		
		TEXCOORDS(tc[2].c4x, tc[2].c4y);
		CALC_AO_Y(+1, +1, +1);
		VERTEX(pos.x+1, pos.y+1, pos.z+1);
#undef BUF_I
	}
	
	if (yn) {
#define BUF_I 3
		// BOTTOM (Y-)
		light = lyn*LIGHT_BOTTOM;
		
		TEXCOORDS(tc[3].c1x, tc[3].c1y);
		CALC_AO_Y(-1, -1, -1);
		VERTEX(pos.x+0, pos.y+0, pos.z+0);
		
		TEXCOORDS(tc[3].c2x, tc[3].c2y);
		CALC_AO_Y(+1, -1, -1);
		VERTEX(pos.x+1, pos.y+0, pos.z+0);
		
		TEXCOORDS(tc[3].c3x, tc[3].c3y);
		CALC_AO_Y(+1, -1, +1);
		VERTEX(pos.x+1, pos.y+0, pos.z+1);
		
		TEXCOORDS(tc[3].c4x, tc[3].c4y);
		CALC_AO_Y(-1, -1, +1);
		VERTEX(pos.x+0, pos.y+0, pos.z+1);
#undef BUF_I
	}
	
	if (zp) {
#define BUF_I 4
		// FRONT (Z+)
		light = lzp*LIGHT_FB;
		
		TEXCOORDS(tc[4].c1x, tc[4].c1y);
		CALC_AO_Z(-1, -1, +1);
		VERTEX(pos.x+0, pos.y+0, pos.z+1);
		
		TEXCOORDS(tc[4].c2x, tc[4].c2y);
		CALC_AO_Z(+1, -1, +1);
		VERTEX(pos.x+1, pos.y+0, pos.z+1);
		
		TEXCOORDS(tc[4].c3x, tc[4].c3y);
		CALC_AO_Z(+1, +1, +1);
		VERTEX(pos.x+1, pos.y+1, pos.z+1);
		
		TEXCOORDS(tc[4].c4x, tc[4].c4y);
		CALC_AO_Z(-1, +1, +1);
		VERTEX(pos.x+0, pos.y+1, pos.z+1);
#undef BUF_I
	}
	
	if (zn) {
#define BUF_I 5
		// BACK (Z-)
		light = lzn*LIGHT_FB;
		
		TEXCOORDS(tc[5].c1x, tc[5].c1y);
		CALC_AO_Z(+1, -1, -1);
		VERTEX(pos.x+1, pos.y+0, pos.z+0);
		
		TEXCOORDS(tc[5].c2x, tc[5].c2y);
		CALC_AO_Z(-1, -1, -1);
		VERTEX(pos.x+0, pos.y+0, pos.z+0);
		
		TEXCOORDS(tc[5].c3x, tc[5].c3y);
		CALC_AO_Z(-1, +1, -1);
		VERTEX(pos.x+0, pos.y+1, pos.z+0);
		
		TEXCOORDS(tc[5].c4x, tc[5].c4y);
		CALC_AO_Z(+1, +1, -1);
		VERTEX(pos.x+1, pos.y+1, pos.z+0);
#undef BUF_I
	}//*/
}

void Chunk::drawTallGrass(sideTexCoords tc, util::vec3f pos) {
#define BUF_I 6
#define l 1
	TEXCOORDS(tc.c1x, tc.c1y);
	GRASS_COLOR(l*1, 0.5);
	VERTEX(pos.x+0.5, pos.y+0, pos.z+1);
	TEXCOORDS(tc.c2x, tc.c2y);
	GRASS_COLOR(l*1, 0.5);
	VERTEX(pos.x+0.5, pos.y+0, pos.z+0);
	TEXCOORDS(tc.c3x, tc.c3y);
	GRASS_COLOR(l*1, 1.0);
	VERTEX(pos.x+0.5, pos.y+1, pos.z+0);
	TEXCOORDS(tc.c4x, tc.c4y);
	GRASS_COLOR(l*1, 1.0);
	VERTEX(pos.x+0.5, pos.y+1, pos.z+1);
	
	TEXCOORDS(tc.c1x, tc.c1y);
	GRASS_COLOR(l*1, 0.5);
	VERTEX(pos.x+1, pos.y+0, pos.z+0.5);
	TEXCOORDS(tc.c2x, tc.c2y);
	GRASS_COLOR(l*1, 0.5);
	VERTEX(pos.x+0, pos.y+0, pos.z+0.5);
	TEXCOORDS(tc.c3x, tc.c3y);
	GRASS_COLOR(l*1, 1.0);
	VERTEX(pos.x+0, pos.y+1, pos.z+0.5);
	TEXCOORDS(tc.c4x, tc.c4y);
	GRASS_COLOR(l*1, 1.0);
	VERTEX(pos.x+1, pos.y+1, pos.z+0.5);
#undef BUF_I
}


void Chunk::draw(World* world, util::vec3f playerPos) {
	if (is_empty) return;
	
	// rebuild vertex buffers (if needed)
	if (needsRedraw) {
		needsRedraw = false;
		
		for (int i = 0; i < 7; i++)
			bufferSizes[i] = 0;
		
		// draw blocks
		blockID blockID;
		blockTexCoords texCoords;
		util::vec3f pos; // stores current block pos (sent to drawBlock to eliminate glTranslate)
		util::vec3f cpos = util::vec3f{static_cast<float>(cx), static_cast<float>(cy), static_cast<float>(cz)}*float(CHUNK_SIZE);
		for (int x = 0; x < CHUNK_SIZE; x++) {
			pos.x = x;
			for (int y = 0; y < CHUNK_SIZE; y++) {
				pos.y = y;
				for (int z = 0; z < CHUNK_SIZE; z++) {
					pos.z = z;
					blockID = blocks[x][y][z];
					if (!blockID || blockID==GRASS_ID) continue;
					
					int wx = x + (cx*CHUNK_SIZE),
						wy = y + (cy*CHUNK_SIZE),
						wz = z + (cz*CHUNK_SIZE);
					if (blockID == TALL_GRASS_ID || blockID == FLOWER_GRASS_ID || blockID == MOUNTAIN_GRASS_ID) {
						drawTallGrass(((TallGrass*)Block::block(blockID))->getSideTexCoords(wx, wy, wz), pos);
					} else {
						drawBlock(world, cpos+pos, Block::block(blockID)->getTexCoords(texCoords, wx, wy, wz, world), pos,
									   Block::block(world->getBlock(wx+1, wy, wz))->isTransparent,
									   Block::block(world->getBlock(wx-1, wy, wz))->isTransparent,
									   Block::block(world->getBlock(wx, wy+1, wz))->isTransparent,
									   Block::block(world->getBlock(wx, wy-1, wz))->isTransparent,
									   Block::block(world->getBlock(wx, wy, wz+1))->isTransparent,
									   Block::block(world->getBlock(wx, wy, wz-1))->isTransparent);
					}
				}
			}
		}
		
		// populate buffers
		if (!buffers[0])
			glGenBuffers(7, buffers);
#define BUFFER_DATA(buf_i)\
		glBindBuffer(GL_ARRAY_BUFFER, buffers[buf_i]);\
		glBufferData(GL_ARRAY_BUFFER, bufferSizes[buf_i]*sizeof(GLfloat), tmp_buffers[buf_i], GL_STATIC_READ);
		BUFFER_DATA(0);
		BUFFER_DATA(1);
		BUFFER_DATA(2);
		BUFFER_DATA(3);
		BUFFER_DATA(4);
		BUFFER_DATA(5);
		BUFFER_DATA(6);
	}
	
	
	// draw vertex buffers
	
	unsigned long tmp_vertices; // used for quad counting
#define DRAW_ARRAY(buf_i)\
	glBindBuffer(GL_ARRAY_BUFFER, buffers[buf_i]);\
	glTexCoordPointer(2, GL_FLOAT, 8*sizeof(GLfloat), (void*)0);\
	glColorPointer(3, GL_FLOAT, 8*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));\
	glVertexPointer(3, GL_FLOAT, 8*sizeof(GLfloat), (void*)(5*sizeof(GLfloat)));\
	glDrawArrays(GL_QUADS, 0, int( tmp_vertices=(bufferSizes[buf_i]/8) ));\
	terrainQuadsRendered += tmp_vertices/4;
	
	if (bufferSizes[0] > 0 && playerPos.x > cx*CHUNK_SIZE) {
		DRAW_ARRAY(0);
	}
	if (bufferSizes[1] > 0 && playerPos.x < cx*CHUNK_SIZE + CHUNK_SIZE) {
		DRAW_ARRAY(1);
	}
	if (bufferSizes[2] > 0 && playerPos.y > cy*CHUNK_SIZE) {
		DRAW_ARRAY(2);
	}
	if (bufferSizes[3] > 0 && playerPos.y < cy*CHUNK_SIZE + CHUNK_SIZE) {
		DRAW_ARRAY(3);
	}
	if (bufferSizes[4] > 0 && playerPos.z > cz*CHUNK_SIZE) {
		DRAW_ARRAY(4);
	}
	if (bufferSizes[5] > 0 && playerPos.z < cz*CHUNK_SIZE + CHUNK_SIZE) {
		DRAW_ARRAY(5);
	}
	if (bufferSizes[6] > 0) {
		glDisable(GL_CULL_FACE);
		DRAW_ARRAY(6);
		glEnable(GL_CULL_FACE);
	}
}


