//
//  World.cpp
//  Bedrock
//
//  Created by Quinn on 9/17/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "World.h"

#include "PerlinNoise.h"
#include "Player.h"
#include "globals.h"
#include "util.h"
#include "frustum.h"
#include "input.h"
#include "shaders.h"

#include "math.h"

const float gravity = 32.0f;

World::World() {
	//printf("PREDICTED WORLD SIZE IN MEMORY: %f MB\n\n", (float)(WORLD_X*WORLD_Y*WORLD_Z*sizeof(blockID))/1048576.0f);
}

World::~World() {
	if (!worldSaved)
		util::saveWorld();
	destruct();
}

static bool* occlusionCache = NULL;
void World::destruct() {
	delete [] occlusionCache; occlusionCache = NULL;
	for (int cx = 0; cx < WORLD_CX; cx++)
		for (int cy = 0; cy < WORLD_CY; cy++)
			for (int cz = 0; cz < WORLD_CZ; cz++) {
				delete chunks[cx][cy][cz];
				chunks[cx][cy][cz] = NULL;
			}
}


//// UPDATING ////

extern long lastFrame;
void World::update(long dt) {
	//srandom((unsigned)lastFrame);
	updateEntities(dt);
	updateTileEntities(dt);
	updateParticles(dt);
}

void World::updateEntities(long dt) {
	for (auto it = entities.begin(); it != entities.end(); ) {
		Entity* e = *(it++);
		if (e->remove) {
			removeEntity(e);
			delete e;
		} else
			e->update(dt);
	}
}

void World::updateTileEntities(long dt) {
	for (std::pair<int,TileEntity*> pair : tileEntities) {
		pair.second->update(dt);
	}
}

void World::updateParticles(long dt) {
	particleCount = particleDrawCount = 0;
	for (int i = 0; i < MAX_PARTICLES; i++) {
#define p particles[i]
		if (p.life > 0) {
			p.update(dt);
			particleCount++;
			if (frustumUtil::isSphereInFrustum(EXPAND_VEC3(p), p.size/2)) {
				particlePositions[particleDrawCount*3+0] = p.x;
				particlePositions[particleDrawCount*3+1] = p.y;
				particlePositions[particleDrawCount*3+2] = p.z;
				particleColorsSizes[particleDrawCount*4+0] = p.color.x;
				particleColorsSizes[particleDrawCount*4+1] = p.color.y;
				particleColorsSizes[particleDrawCount*4+2] = p.color.z;
				particleColorsSizes[particleDrawCount*4+3] = p.size;
				particleDrawCount++;
			}
		}
#undef p
	}
}

bool World::addParticle(int type, float x, float y, float z, bool old) {
	// find an unused slot
	bool foundSlot = false;
	for (int i = lastParticleSlot; i < MAX_PARTICLES; i++)
        if (particles[i].life < 0){
            lastParticleSlot = i;
            foundSlot = true; break;
        }
    if (!foundSlot) for (int i = 0; i < lastParticleSlot; i++)
        if (particles[i].life < 0){
            lastParticleSlot = i;
            foundSlot = true; break;
        }
	if (!foundSlot) {
		//printf("PARTICLES MAXXED AT %i\n", MAX_PARTICLES);
		return false;
	}
	
	// add particle at slot
	particles[lastParticleSlot] = Particle(type, x, y, z, old);
	return true;
}

//// DRAWING ////

void World::draw() {
	if (input::debug) {
		glUniform1i(shaders::fog_debugLoc, true);
	}
	drawChunks({curUnit->x,curUnit->y,curUnit->z}, {EXPAND_VEC3(player)});
	drawEntities();
	drawParticles();
	if (input::debug) {
		glUniform1i(shaders::fog_debugLoc, false);
	}
}

/*// used for raytracing
//occlusionCache
static int occCacheX, occCacheY, occCacheZ, occCacheXY;
static util::vec3i oldcmin, oldcmax;
static bool isOccluder(int x, int y, int z, util::vec3f face) {
	return occlusionCache[x+(occCacheX*y)+(occCacheXY*z)];
}*/
void World::drawChunks(const util::vec3f unitPos, const util::vec3f playerPos) {
	const float chunkRenderRadius = ceilf(curUnit->viewDistance/CHUNK_SIZE);
	int cxmin = floor( (unitPos.x/CHUNK_SIZE)-chunkRenderRadius ),
		cxmax = floor( (unitPos.x/CHUNK_SIZE)+chunkRenderRadius ),
		
		cymin = floor( (unitPos.y/CHUNK_SIZE)-chunkRenderRadius ),
		cymax = floor( (unitPos.y/CHUNK_SIZE)+chunkRenderRadius ),
		
		czmin = floor( (unitPos.z/CHUNK_SIZE)-chunkRenderRadius ),
		czmax = floor( (unitPos.z/CHUNK_SIZE)+chunkRenderRadius );
	
	/*const util::vec3f occ_playerPos = playerPos/OCCLUSION_CELL_SIZE;
	// fill occlusion cache
	util::vec3i newcmin={cxmin,cymin,czmin},newcmax={cxmax,cymax,czmax}, occCacheOffset = newcmin*OCCLUSION_RESOLUTION;
	if (newcmin!=oldcmin || newcmax!=oldcmax) {
		oldcmin = newcmin; oldcmax = newcmax;
		delete [] occlusionCache;
		occCacheX=(cxmax-cxmin+1)*OCCLUSION_RESOLUTION;occCacheY=(cymax-cymin+1)*OCCLUSION_RESOLUTION;occCacheZ=(czmax-czmin+1)*OCCLUSION_RESOLUTION;
		occlusionCache = new bool[(occCacheXY=occCacheX*occCacheY)*occCacheZ];
		for (int ocx = 0; ocx < occCacheX; ocx++) {
			for (int ocy = 0; ocy < occCacheY; ocy++) {
				for (int ocz = 0; ocz < occCacheZ; ocz++) {
					occlusionCache[ocx+(occCacheX*ocy)+(occCacheXY*ocz)] =
					getOcclusionValue(ocx+occCacheOffset.x, ocy+occCacheOffset.y, ocz+occCacheOffset.z) == OCC_FULL;
				}
			}
		}
	}*/
	
	chunkSet tmpVisibleChunks;
	
	// setup OpenGL states
	glUniform1i(shaders::fog_terrainLoc, true);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	if (input::debug) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else glBindTexture(GL_TEXTURE_2D, textures::blocks);
	terrainQuadsRendered = 0;
	
	for (int cx = cxmin; cx <= cxmax; cx++)
		for (int cy = cymin; cy <= cymax; cy++)
			for (int cz = czmin; cz <= czmax; cz++) {
				
				int tcx = (cx + WORLD_CX) % WORLD_CX,
					tcy = (cy + WORLD_CY) % WORLD_CY,
					tcz = (cz + WORLD_CZ) % WORLD_CZ;
				
				Chunk * chunk = chunks[tcx][tcy][tcz];
				if (!chunk) continue;
				tmpVisibleChunks.insert(chunk);
				//if (chunk->needsRedraw) oldcmax = {9001,0,0}; // rebuild occlusion cache
				
				// draw all the chunks first frame to cache meshes
				if (!firstDraw)  {
					using util::vec3f; using util::vec3i;
					// don't bother with chunks that don't have any geometry
					int quadCount = chunk->quadCount(playerPos);
					if (quadCount==0 && !chunk->needsRedraw) continue;
					
					// cull chunks outside of view frustum
					const vec3f chunkCenter = vec3f{cx,cy,cz}*CHUNK_SIZE+(CHUNK_SIZE/2);
					if (!frustumUtil::isCubeInFrustum(EXPAND_VEC3(chunkCenter), CHUNK_SIZE/2))
						continue;
					
					/*// raytrace occlusion culling
					if (quadCount > 75) {
						const vec3i occ_chunkPos = util::vec3i{chunk->cx,chunk->cy,chunk->cz}*OCCLUSION_RESOLUTION;
						vec3i ocPos;
						vec3f toOC;
						bool visible = false;
						for (int ocx = 0; !visible && ocx < OCCLUSION_RESOLUTION; ocx++) {
							ocPos.x = ocx+occ_chunkPos.x;
							for (int ocy = 0; !visible && ocy < OCCLUSION_RESOLUTION; ocy++) {
								ocPos.y = ocy+occ_chunkPos.y;
								for (int ocz = 0; !visible && ocz < OCCLUSION_RESOLUTION; ocz++) {
									ocPos.z = ocz+occ_chunkPos.z;
									if (world.getOcclusionValue(EXPAND_VEC3(ocPos)) == OCC_NONE) continue;
									vec3i hit = util::rayTrace(occ_playerPos-vec3f(occCacheOffset), toOC=(vec3f(ocPos)+0.5)-occ_playerPos, toOC.length()+0.1, isOccluder);
									if (hit==vec3i(ocPos) || hit==vec3i{-1,-1,-1})
										visible = true;
								}
							}
						}
						if (!visible) continue;
					}*/
					
					// cull chunks outside of sphere
					static const float CHUNK_DIAG = sqrtf( (CHUNK_SIZE/2)*(CHUNK_SIZE/2)*3 );
					if ( (chunkCenter - unitPos).length() > curUnit->viewDistance+CHUNK_DIAG )
						continue;
				}
				
				glPushMatrix();
				{
				#ifdef WORLD_VIEW
					glTranslatef(tcx*CHUNK_SIZE, tcy*CHUNK_SIZE, tcz*CHUNK_SIZE);
				#else
					glTranslatef(unitPos.x-(tcx*CHUNK_SIZE) > WORLD_X/2 ?		tcx*CHUNK_SIZE+WORLD_X :
								 unitPos.x-(tcx*CHUNK_SIZE) < -WORLD_X/2 ?	tcx*CHUNK_SIZE-WORLD_X :
																			tcx*CHUNK_SIZE,
								 
								 unitPos.y-(tcy*CHUNK_SIZE) > WORLD_Y/2 ?		tcy*CHUNK_SIZE+WORLD_Y :
								 unitPos.y-(tcy*CHUNK_SIZE) < -WORLD_Y/2 ?	tcy*CHUNK_SIZE-WORLD_Y :
																			tcy*CHUNK_SIZE,
								 
								 unitPos.z-(tcz*CHUNK_SIZE) > WORLD_Z/2 ?		tcz*CHUNK_SIZE+WORLD_Z :
								 unitPos.z-(tcz*CHUNK_SIZE) < -WORLD_Z/2 ?	tcz*CHUNK_SIZE-WORLD_Z :
																			tcz*CHUNK_SIZE);
				#endif
					
					chunk->draw(this, playerPos);
				}
				glPopMatrix();
				
			}
	
	// clear OpenGL states
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glUniform1i(shaders::fog_terrainLoc, false);
	if (input::debug) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (firstDraw)
		firstDraw = false;
	
	// update visible state on Chunks
	for (Chunk * c : tmpVisibleChunks) {
		if (visibleChunks.find(c) == visibleChunks.end())
			c->onVisible();
	}
	for (Chunk * c : visibleChunks) {
		if (tmpVisibleChunks.find(c) == tmpVisibleChunks.end())
			c->onInvisible();
	}
	visibleChunks = tmpVisibleChunks;
}

extern Entity * entitySelection;
void World::drawEntities() {
	for (Entity* e : entities) {
#define max(a,b) (a>b?a:b)
		if (frustumUtil::isCubeInFrustum(e->x, e->y, e->z, max(max(e->rx,e->ry),e->rz))) {
			if (entitySelection==e) glUniform1i(shaders::fog_selectLoc, true);
			e->draw();
			if (entitySelection==e) glUniform1i(shaders::fog_selectLoc, false);
		}
		
#undef max
	}
}

void World::drawParticles() {
	glUniform1i(shaders::fog_texturesLoc, false); // disable texturing
	//////////////////////////////////////////
	
	
	// vertex data
	glEnableClientState(GL_VERTEX_ARRAY);
	static const GLfloat vertices[] = {
		0.5, -0.5,
		0.5, 0.5,
		-0.5, 0.5,
		-0.5, -0.5,
	};
	glVertexPointer(2/*coords per vertex*/, GL_FLOAT, 0/*bytes between vertices*/, vertices/*data*/);
	
	// position data
	glEnableVertexAttribArray(shaders::fog_facePositionLoc);
	glVertexAttribPointer(shaders::fog_facePositionLoc, 3, GL_FLOAT, GL_FALSE, 0, particlePositions);
	glVertexAttribDivisorARB(shaders::fog_facePositionLoc, 1); // 1 position per instance
	
	// color/size data
	glEnableVertexAttribArray(shaders::fog_particleColorSizeLoc);
	glVertexAttribPointer(shaders::fog_particleColorSizeLoc, 4, GL_FLOAT, GL_FALSE, 0, particleColorsSizes);
	glVertexAttribDivisorARB(shaders::fog_particleColorSizeLoc, 1); // 1 color/size per instance
	
	// draw the primitives
	glDrawArraysInstancedARB(GL_QUADS, 0, 4, particleDrawCount);
	
	// disable stuff
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableVertexAttribArray(shaders::fog_facePositionLoc);
	glDisableVertexAttribArray(shaders::fog_particleColorSizeLoc);
	
	
	//////////////////////////////////////////
	glUniform1i(shaders::fog_texturesLoc, true);
}

/////////////////


Chunk * World::loadChunk(int cx, int cy, int cz) {
	Chunk * chunk = new Chunk(cx, cy, cz, this);
	chunks[cx][cy][cz] = chunk;
	return chunk;
}

blockID World::getBlock(int x, int y, int z) {
	x += WORLD_X;
	y += WORLD_Y;
	z += WORLD_Z;
	int cx = (x/CHUNK_SIZE)%WORLD_CX,
		cy = (y/CHUNK_SIZE)%WORLD_CY,
		cz = (z/CHUNK_SIZE)%WORLD_CZ;
	Chunk * chunk = chunks[cx][cy][cz];
	
	if (!chunk) {
		//chunk = loadChunk(cx, cy, cz);
		return AIR_ID;
	}
	
	return chunk->getBlock(	x%CHUNK_SIZE,
							y%CHUNK_SIZE,
							z%CHUNK_SIZE);
}

occVal World::getOcclusionValue(int ocx, int ocy, int ocz) {
	Chunk * c = getChunk(ocx/OCCLUSION_RESOLUTION, ocy/OCCLUSION_RESOLUTION, ocz/OCCLUSION_RESOLUTION);
	if (!c || !c->occlusionChunk) return OCC_NONE;
	return c->occlusionChunk->getValue(ocx%OCCLUSION_RESOLUTION, ocy%OCCLUSION_RESOLUTION, ocz%OCCLUSION_RESOLUTION);
}

void World::setBlock(int x, int y, int z, blockID b) {
	x += WORLD_X;
	y += WORLD_Y;
	z += WORLD_Z;
	int cx = (x/CHUNK_SIZE)%WORLD_CX,
	cy = (y/CHUNK_SIZE)%WORLD_CY,
	cz = (z/CHUNK_SIZE)%WORLD_CZ;
	Chunk * chunk = chunks[cx][cy][cz];
	
	if (!chunk) {
		chunk = loadChunk(cx, cy, cz);
	}
	
	chunk->setBlock(x%CHUNK_SIZE, y%CHUNK_SIZE, z%CHUNK_SIZE, b);
}

void World::setBlockWithNotify(int x, int y, int z, blockID b) {
	triggerInvisible(x, y, z);
	setBlock(x, y, z, b);
	triggerBlockUpdate(x+1, y, z, x, y, z);
	triggerBlockUpdate(x-1, y, z, x, y, z);
	triggerBlockUpdate(x, y+1, z, x, y, z);
	triggerBlockUpdate(x, y-1, z, x, y, z);
	triggerBlockUpdate(x, y, z+1, x, y, z);
	triggerBlockUpdate(x, y, z-1, x, y, z);
	triggerVisible(x, y, z);
}

#define TILEENTITYKEY(x,y,z) ((x+WORLD_X)%WORLD_X)+((y+WORLD_Y)%WORLD_Y)*WORLD_X+((z+WORLD_Z)%WORLD_Z)*WORLD_X*WORLD_Y
TileEntity* World::getTileEntity(int x, int y, int z) {
	int key = TILEENTITYKEY(x,y,z);
	if (tileEntities.find(key) == tileEntities.end())
		return NULL; // no TileEntity at (x,y,z)
	return tileEntities[key];
}
bool World::setTileEntity(TileEntity* te, int x, int y, int z) {
	int key = TILEENTITYKEY(x,y,z);
	if (tileEntities.find(key) != tileEntities.end())
		return false; // there is already a TileEntity at (x,y,z)
	tileEntities[key] = te;
	return true;
}
void World::deleteTileEntity(int x, int y, int z) {
	int key = TILEENTITYKEY(x,y,z);
	if (tileEntities.find(key) == tileEntities.end())
		return; // no TileEntity at (x,y,z)
	delete tileEntities.at(key);
	tileEntities.erase(key);
}
#undef TILEENTITYKEY


