//
//  worldGen.cpp
//  Bedrock
//
//  Created by Quinn on 11/15/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "worldGen.h"

#include "PerlinNoise.h"
#include "globals.h"
#include "ortho.h"
#include "util.h"

#include "math.h"

static inline bool isDirt(blockID block) {
	return block==DIRT_ID || block==ROCKY_DIRT_ID;
}
static inline bool isClay(blockID block) {
	return block==CLAY_ID || block==ROCKY_CLAY_ID;
}

static inline blockID randDirt() {
	return random()%10 ? DIRT_ID : ROCKY_DIRT_ID;
}
static inline blockID randClay() {
	return random()%10 ? CLAY_ID : ROCKY_CLAY_ID;
}

static inline blockID randTallGrass() {
	return random()%7 ? TALL_GRASS_ID : FLOWER_GRASS_ID;
}

static unsigned int curSeed;

typedef blockID (*fillFunc)(int,int,int);

static void placeRoughSphere(int cx, int cy, int cz, int radius, fillFunc blockType, bool overwrite) {
#define PERLIN_SPHERE_SCALE 5.0f
	PerlinNoise perlinSphere(PERLIN_SPHERE_SCALE, PERLIN_SPHERE_SCALE, // width, height
							  2,			// octaves
							  1.5,			// persistence
							  curSeed++,	// seed
							  NoiseGrid::Interpolation::COSINE);	// interpolation type
	util::vec3f pos;
	util::vec3f center = {static_cast<float>(cx), static_cast<float>(cy), static_cast<float>(cz)};
#define PADDING 3
	for (int x = cx-radius-PADDING; x < cx+radius+PADDING; x++) {
		pos.x = x;
		for (int y = cy-radius-PADDING; y < cy+radius+PADDING; y++) {
			pos.y = y;
			for (int z = cz-radius-PADDING; z < cz+radius+PADDING; z++) {
				if (!overwrite && Block::block(world.getBlock(x, y, z))->isSolid)
					continue;
				
				pos.z = z;
				util::vec3f toCenter = pos-center;
				float yaw = atan2f(-toCenter.y, toCenter.x)/DEG_TO_RAD;
				if (yaw<0) yaw+=360;
				float pitch = atan2f(toCenter.z, sqrtf((toCenter.x*toCenter.x) + (toCenter.y*toCenter.y)))/DEG_TO_RAD;
				if (pitch<0) pitch+=360;
				if (toCenter.length() <= radius+
					perlinSphere.getNoise(yaw/360*PERLIN_SPHERE_SCALE,
										   pitch/360*PERLIN_SPHERE_SCALE))
					world.setBlockWithNotify(x, y, z, blockType(x, y, z));
			}
		}
	}
#undef PADDING
}

// fill functions for placeRoughSphere //
template <blockID blockType>
static blockID solidFill(int x, int y, int z) {
	return blockType;
}
template <blockID block1, blockID block2, int chance>
static blockID randFill(int x, int y, int z) {
	return (random()%100)<chance? block2 : block1;
}
/////////////////////////////////////////


void worldGen::generateWorld(World & world, unsigned int seed) {
	ortho::drawLoadingScreen("Generating new world", "Initializing");
	printf("Generating new world with seed %i...\n", seed);
	
	curSeed = seed;
	
#define PERLIN_TOP_SCALE 12.0f
	PerlinNoise perlinMountains(WORLD_CX/PERLIN_TOP_SCALE+1, WORLD_CZ/PERLIN_TOP_SCALE+1,	 // width, height
						  4,		// octaves
						  1.9,		// persistence
						  curSeed++,// seed
								NoiseGrid::Interpolation::COSINE);	// interpolation type
	
	PerlinNoise perlinFlat(WORLD_X/16, WORLD_Z/16, // width, height
							2,			// octaves
							0.5,		// persistence
							curSeed++,	// seed
							NoiseGrid::Interpolation::COSINE);	// interpolation type
	
	PerlinNoise perlinBottom(WORLD_CX/4, WORLD_CZ/4, // width, height
							 4,			// octaves
							 2.0,		// persistence
							 curSeed++,	// seed
							 NoiseGrid::Interpolation::LINEAR);	// interpolation type
	
	PerlinNoise perlinDirt(WORLD_X/16, WORLD_Z/16, // width, height
							3,			// octaves
							0.9,		// persistence
							curSeed++,	// seed
							NoiseGrid::Interpolation::COSINE);	// interpolation type
	
	PerlinNoise perlinClay(WORLD_X/16, WORLD_Z/16, // width, height
							3,			// octaves
							1.4,		// persistence
							curSeed++,	// seed
							NoiseGrid::Interpolation::COSINE);	// interpolation type
	
	PerlinNoise perlinGrass(WORLD_X/16, WORLD_Z/16, // width, height
							3,			// octaves
							1.4,		// persistence
							curSeed++,	// seed
							NoiseGrid::Interpolation::COSINE);	// interpolation type
	
	PerlinNoise perlinTrees(WORLD_X/64, WORLD_Z/64, // width, height
							2,			// octaves
							1.4,		// persistence
							curSeed++,	// seed
							NoiseGrid::Interpolation::COSINE);	// interpolation type
	
	
	ortho::drawLoadingScreen("Generating new world", "Forming island");
	printf("Forming island...\n");
	int heightMap[WORLD_X][WORLD_Z]; // stores heightmap for subsequent operations
	for (int x = 0; x < WORLD_X; x++) {
		//printf("Loading: %i\n", int(float(x)/float(WORLD_X) * 100));
		for (int z = 0; z < WORLD_Z; z++) {
			int xDist = x-(WORLD_X/2), zDist = z-(WORLD_Z/2);
			float distFromCenter = sqrt( (xDist*xDist) + (zDist*zDist) );
			
			int ymax = perlinMountains.getNoise(x/float(CHUNK_SIZE)/PERLIN_TOP_SCALE, z/float(CHUNK_SIZE)/PERLIN_TOP_SCALE)*5;
			ymax += (WORLD_Y/2)+175 - distFromCenter/2;
			int flatVal = perlinFlat.getNoise(x/16.0, z/16.0) * 10.0f;
#define Y_FILL 383
			if (ymax < Y_FILL+flatVal) ymax = Y_FILL+flatVal;
			int ymin = -perlinBottom.getNoise(x/float(CHUNK_SIZE)/4, z/float(CHUNK_SIZE)/4)*10;
			ymin += (WORLD_Y/2)-128 + distFromCenter*1.3;
			
			if (ymax-ymin >= 5) {// no dirt if stone isnt underneath
				int dirtThickness = perlinDirt.getNoise(x/16.0, z/16.0)*3;
				bool clay = perlinClay.getNoise(x/16.0, z/16.0) > 2.9; // 2.9 = clay threshold
#define NO_DIRT_Y (Y_FILL+20)
				if (ymax > NO_DIRT_Y) dirtThickness -= (ymax - NO_DIRT_Y)/10;
				for (int i = ymin; i <= ymax; i++) {
					world.setBlock(x, i, z,
					i>=ymax-dirtThickness+2? (clay?randClay():randDirt()): i>=ymax-dirtThickness?(clay?ROCKY_CLAY_ID:ROCKY_DIRT_ID): STONE_ID);
				}
				heightMap[x][z] = ymax;
			} else heightMap[x][z] = 0;
		}
	}
	
	
	ortho::drawLoadingScreen("Generating new world", "Smoothing terrain");
	printf("Smoothing terrain...\n");
	for (int x = 0; x < WORLD_X; x++) {
		for (int z = 0; z < WORLD_Z; z++) {
			if (heightMap[x][z] == 0) continue;
			// remove one block bumps
			if (	(heightMap[x][z] > heightMap[x+1][z]) +
					(heightMap[x][z] > heightMap[x-1][z]) +
					(heightMap[x][z] > heightMap[x][z+1]) +
					(heightMap[x][z] > heightMap[x][z-1]) >= 3) {
				
				world.setBlock(x, heightMap[x][z]--, z, 0);
			}
			// fill one block holes
			if (	(heightMap[x][z] < heightMap[x+1][z]) +
					(heightMap[x][z] < heightMap[x-1][z]) +
					(heightMap[x][z] < heightMap[x][z+1]) +
					(heightMap[x][z] < heightMap[x][z-1]) >= 3) {
				
				int dirtThickness = perlinDirt.getNoise(x/16.0, z/16.0)*3;
				if (heightMap[x][z] > NO_DIRT_Y) dirtThickness -= (heightMap[x][z] - NO_DIRT_Y)/10;
				world.setBlock(x, ++heightMap[x][z], z, dirtThickness >= 2? randDirt(): dirtThickness >= 0? ROCKY_DIRT_ID: STONE_ID);
			}
		}
	}//*/
	
	
	ortho::drawLoadingScreen("Generating new world", "Planting grass");
	printf("Planting grass...\n");
	bool dirtBelow;
	for (int x = 0; x < WORLD_X; x++) {
		for (int z = 0; z < WORLD_Z; z++) {
			float grass = perlinGrass.getNoise(x/16.0, z/16.0);
#define GRASS_THRESH 1.5 //2.0
#define TALL_GRASS_THRESH 100 //2.0 //2.5
			if (grass > GRASS_THRESH) {
				for (int y = heightMap[x][z]+1; y < WORLD_Y; y++) {
					if (isDirt(world.getBlock(x+1, y, z)) ||
						isDirt(world.getBlock(x-1, y, z)) ||
						isDirt(world.getBlock(x, y, z+1)) ||
						isDirt(world.getBlock(x, y, z-1)) ||
						(dirtBelow = isDirt(world.getBlock(x, y-1, z)))) {
						
						world.setBlock(x, y, z, grass>TALL_GRASS_THRESH && y==heightMap[x][z]+1 && dirtBelow && random()%10<5? randTallGrass() : GRASS_ID);
					} else break;
				}
				// mountain grass
				if (grass>TALL_GRASS_THRESH &&
					random()%4 == 0 &&
					isClay(world.getBlock(x, heightMap[x][z], z))) {
					world.setBlock(x, heightMap[x][z]+1, z, MOUNTAIN_GRASS_ID);
				}
			}
		}
	}
	
	
	ortho::drawLoadingScreen("Generating new world", "Carving rivers");
	printf("Carving rivers...\n");
	for (int n = 0; n < 50; n++) {
		int x = random() % WORLD_X;
		int z = random() % WORLD_Z;
		int curHeight;
		int flatCount = 0;
		int lastDir = -1;
		while ((curHeight=heightMap[x][z]) != 0) {
			world.setBlockWithNotify(x, curHeight, z, AIR_ID);
			world.setBlockWithNotify(x, curHeight-1, z, WATER_ID);
			//heightMap[x][z] -= 2;
			
			// move downhill
			int heights[4] = {heightMap[x+1][z],heightMap[x-1][z],heightMap[x][z+1],heightMap[x][z-1]};
			bool downhills[4] = {true,true,true,true};
			int numDownhills = 4;
			bool flat = true;
			for (int i1 = 0; i1 < 4; i1++) {
				if (heights[i1] > curHeight || i1 == lastDir) {
					downhills[i1] = false; numDownhills--;
					continue;
				}
				if (heights[i1] != curHeight)
					flat = false;
				for (int i2 = 0; i2 < 4; i2++) {
					if (i2 != lastDir && heights[i1] > heights[i2]) {
						downhills[i1] = false; numDownhills--;
						break;
					}
				}
			}
			
			if (flat) flatCount++;
			else flatCount = 0;
			
			if (numDownhills == 0 || flatCount >= 10)
				break;
			int selectedDownhill = (random() % numDownhills) + 1;
			for (int i = 0, c = 0; i < 4; i++) {
				if (downhills[i]) c++;
				if (c == selectedDownhill) {
					if (i==0) {x++;lastDir=1;}
					if (i==1) {x--;lastDir=0;}
					if (i==2) {z++;lastDir=3;}
					if (i==3) {z--;lastDir=2;}
					break;
				}
			}
		}
	}
	
	
	ortho::drawLoadingScreen("Generating new world", "Placing boulders");
	printf("Placing boulders...\n");
	for (int n = 0; n < 400; n++) {
		int bx = random()%WORLD_X,
			bz = random()%WORLD_Y,
			by = heightMap[bx][bz] + 1 + random()%30;
		if (by == 1) continue;
		
		int radius = random()%5 + 1; // 1-5
		by -= radius/2;
		
		placeRoughSphere(bx, by, bz, radius, solidFill<STONE_ID>, true);
	}
	
	
	/*ortho::drawLoadingScreen("Generating new world", "Growing trees");
	printf("Growing trees...\n");
	for (int n = 0; n < 2500; n++) {
		int tx = random()%(WORLD_X-1),
			tz = random()%(WORLD_Y-1),
			ty = heightMap[tx][tz] + 1;
		if (ty == 1) continue;
		if ((random()%90) - (ty-Y_FILL) < 0) continue;
		if (perlinTrees.getNoise(tx/64, tz/64) < 1.1) continue;
		if (world.getBlock(tx, ty-1, tz) == STONE_ID) continue;
		
		blockID woodID;
		fillFunc leavesFill;
		if (random()%3) {
			woodID = BIRCH_WOOD_ID;
			leavesFill = randFill<BIRCH_LEAVES_ID, BIRCH_LEAVES2_ID, 20>;
		} else {
			woodID = WOOD_ID;
			leavesFill = randFill<LEAVES_ID, LEAVES2_ID, 20>;
		}
		
		if (random()%10) {
			// normal tree
			int tsize = random()%10+4;
			
			// trunk
			for (int y = 0; y < tsize; y++)
				world.setBlockWithNotify(tx, ty+y, tz, woodID);
			// leaves
			placeRoughSphere(tx, ty+tsize, tz, tsize/3, leavesFill, false);
		} else {
			// huge tree
			int tsize = random()%20+12;
			if (heightMap[tx+1][tz]<ty-1) ty=heightMap[tx+1][tz]+1;
			if (heightMap[tx][tz+1]<ty-1) ty=heightMap[tx][tz+1]+1;
			if (heightMap[tx+1][tz+1]<ty-1) ty=heightMap[tx+1][tz+1]+1;
			if (ty == 1) continue;
			
			// trunk
			for (int y = 0; y < tsize; y++) {
				world.setBlockWithNotify(tx, ty+y, tz, woodID);
				world.setBlockWithNotify(tx+1, ty+y, tz, woodID);
				world.setBlockWithNotify(tx, ty+y, tz+1, woodID);
				world.setBlockWithNotify(tx+1, ty+y, tz+1, woodID);
			}
			// leaves
			placeRoughSphere(tx, ty+(tsize*0.75), tz, tsize/3, leavesFill, false);
			placeRoughSphere(tx, ty+tsize, tz, tsize/4, leavesFill, false);
		}
	}//*/
	
	
	ortho::drawLoadingScreen("Generating new world", "Done");
	printf("Done.\n\n");
}
