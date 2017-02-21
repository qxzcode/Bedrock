//
//  util.cpp
//  Bedrock
//
//  Created by Quinn on 9/14/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "util.h"

#include "textures.h"
#include "shaders.h"
#include "globals.h"
#include "ortho.h"
#include "Block.h"

#include "math.h"
#include <vector>
#include <iostream>
#include <fstream>
#include </Library/Frameworks/SDL.framework/Headers/SDL.h>
//#include <zlib.h> // unused

void util::swapBuffers() {
	SDL_GL_SwapBuffers();
}

float intbound(float s, float ds) {
	// Find the smallest positive t such that s+t*ds is an integer.
	if (ds < 0) {
		return intbound(-s, -ds);
	} else {
		float derp;
		s = modff(s, &derp);
		if (s < 0) s += 1.0f;
		// problem is now s+t*ds = 1
		return (1-s)/ds;
	}
}

util::vec3i util::rayTrace(vec3f origin, vec3f direction, float radius, bool callback(int,int,int,vec3f)) {
	// From "A Fast Voxel Traversal Algorithm for Ray Tracing"
	// by John Amanatides and Andrew Woo, 1987
	// <http://www.cse.yorku.ca/~amana/research/grid.pdf>
	// <http://citeseer.ist.psu.edu/viewdoc/summary?doi=10.1.1.42.3443>
	// Extensions to the described algorithm:
	//   • Imposed a distance limit.
	//   • The face passed through to reach the current cube is provided to
	//     the callback.
	
	// The foundation of this algorithm is a parameterized representation of
	// the provided ray,
	//                    origin + t * direction,
	// except that t is not actually stored; rather, at any given point in the
	// traversal, we keep track of the *greater* t values which we would have
	// if we took a step sufficient to cross a cube boundary along that axis
	// (i.e. change the integer part of the coordinate) in the variables
	// tMaxX, tMaxY, and tMaxZ.
	
	// Cube containing origin point.
	int x = floorf(origin.x);
	int y = floorf(origin.y);
	int z = floorf(origin.z);
	// Break out direction vector.
	float dx = direction.x;
	float dy = direction.y;
	float dz = direction.z;
	// Direction to increment x,y,z when stepping.
	int stepX = sign(dx);
	int stepY = sign(dy);
	int stepZ = sign(dz);
	// See description above. The initial values depend on the fractional
	// part of the origin.
	float tMaxX = intbound(origin.x, dx);
	float tMaxY = intbound(origin.y, dy);
	float tMaxZ = intbound(origin.z, dz);
	// The change in t when taking a step (always positive).
	float tDeltaX = dx==0? 0 : stepX/dx;
	float tDeltaY = dy==0? 0 : stepY/dy;
	float tDeltaZ = dz==0? 0 : stepZ/dz;
	// Buffer for reporting faces to the callback.
	vec3f face = {0,0,0};
	
	// Avoids an infinite loop.
	if (dx == 0 && dy == 0 && dz == 0) {
		printf("Raycast in zero direction!\n");
		return {-1, -1, -1};
	}
		
	// Rescale from units of 1 cube-edge to units of 'direction' so we can
	// compare with 't'.
	radius /= sqrtf(dx*dx + dy*dy + dz*dz);
	
	bool hitBlock = false;
	
	while (true) {
		
		// Invoke the callback (should return true to stop traversing)
		if (callback(x, y, z, face)) {
			hitBlock = true;
			break;
		}
		
		// tMaxX stores the t-value at which we cross a cube boundary along the
		// X axis, and similarly for Y and Z. Therefore, choosing the least tMax
		// chooses the closest cube boundary. Only the first case of the four
		// has been commented in detail.
		if (tMaxX < tMaxY) {
			if (tMaxX < tMaxZ) {
				if (tMaxX > radius) break;
				// Update which cube we are now in.
				x += stepX;
				// Adjust tMaxX to the next X-oriented boundary crossing.
				tMaxX += tDeltaX;
				// Record the normal vector of the cube face we entered.
				face.x = -stepX;
				face.y = 0;
				face.z = 0;
			} else {
				if (tMaxZ > radius) break;
				z += stepZ;
				tMaxZ += tDeltaZ;
				face.x = 0;
				face.y = 0;
				face.z = -stepZ;
			}
		} else {
			if (tMaxY < tMaxZ) {
				if (tMaxY > radius) break;
				y += stepY;
				tMaxY += tDeltaY;
				face.x = 0;
				face.y = -stepY;
				face.z = 0;
			} else {
				// Identical to the second case, repeated for simplicity in
				// the conditionals.
				if (tMaxZ > radius) break;
				z += stepZ;
				tMaxZ += tDeltaZ;
				face.x = 0;
				face.y = 0;
				face.z = -stepZ;
			}
		}
	}
	
	if (hitBlock)
		return {x, y, z};
	else return {-1, -1, -1};
}

util::vec3f util::rayToAABB(AABB box, vec3f rayOrigin, vec3f rayDir) {
#define RIGHT	0
#define LEFT	1
#define MIDDLE	2
	
	double minB[3]={box.cx-box.rx,box.cy-box.ry,box.cz-box.rz}, maxB[3]={box.cx+box.rx,box.cy+box.ry,box.cz+box.rz}; /*box */
	double origin[3]={EXPAND_VEC3(rayOrigin)}, dir[3]={EXPAND_VEC3(rayDir)};		/*ray */
	double coord[3];				/* hit point */
	
	bool inside = true;
	char quadrant[3];
	register int i;
	int whichPlane;
	double maxT[3];
	double candidatePlane[3];
	
	/* Find candidate planes; this loop can be avoided if
	 rays cast all from the eye(assume perpsective view) */
	for (i=0; i<3; i++)
		if(origin[i] < minB[i]) {
			quadrant[i] = LEFT;
			candidatePlane[i] = minB[i];
			inside = false;
		}else if (origin[i] > maxB[i]) {
			quadrant[i] = RIGHT;
			candidatePlane[i] = maxB[i];
			inside = false;
		}else	{
			quadrant[i] = MIDDLE;
		}
	
	/* Ray origin inside bounding box */
	if (inside)	{
		return rayOrigin;
	}
	
	
	/* Calculate T distances to candidate planes */
	for (i = 0; i < 3; i++)
		if (quadrant[i] != MIDDLE && dir[i] !=0.)
			maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
		else
			maxT[i] = -1.;
	
	/* Get largest of the maxT's for final choice of intersection */
	whichPlane = 0;
	for (i = 1; i < 3; i++)
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;
	
	/* Check final candidate actually inside box */
	if (maxT[whichPlane] < 0.) return {-1,-1,-1};
	for (i = 0; i < 3; i++)
		if (whichPlane != i) {
			coord[i] = origin[i] + maxT[whichPlane] *dir[i];
			if (coord[i] < minB[i] || coord[i] > maxB[i])
				return {-1,-1,-1};
		} else {
			coord[i] = candidatePlane[i];
		}
	return {coord[0],coord[1],coord[2]};				/* ray hits box */
}

bool util::intersects(const AABB & a, const AABB & b) {
	if ( fabs(a.cx - b.cx) >= (a.rx + b.rx) ) return false;
	if ( fabs(a.cy - b.cy) >= (a.ry + b.ry) ) return false;
	if ( fabs(a.cz - b.cz) >= (a.rz + b.rz) ) return false;
	return true;
}

float util::mod(float f1, float f2) {
	float d = f1/f2;
	
	return (d-int(d)) * f2;
}

struct chunkInfo {
	int cx, cy, cz;
	int bytesLen;
};
struct chunkData {
	char * bytes;
	unsigned long len;
};

void util::saveWorld() {
	ortho::drawLoadingScreen("Saving world", "");
	std::cout << "Saving world..." << std::endl;
	
	// create data directory
	std::string dataDir = getDataDir();
	if (createDirectory(dataDir))
		std::cout << "Created directory " << dataDir << std::endl;
	else if (errno != EEXIST) {
		ortho::drawLoadingScreen("Saving world", "Error: Could not create directory: "+dataDir);
		std::cout << "Error: Could not create directory: " << dataDir << std::endl;
		return;
	}
	
	// get and compress bytes
	ortho::drawLoadingScreen("Saving world", "Compressing data");
	std::cout << "Compressing data..." << std::endl;
	std::vector<chunkData> chunkDatas;
	std::vector<chunkInfo> chunkInfos;
	
	unsigned long totalUncompressed = 0;
	unsigned long totalCompressed = 0;
	int chunkCount = 0;
	
	char chunkBytes[CHUNK_BYTES];
	char rleBytes[CHUNK_BYTES*2];
	for (int cx = 0; cx < WORLD_CX; cx++)
		for (int cy = 0; cy < WORLD_CY; cy++)
			for (int cz = 0; cz < WORLD_CZ; cz++) {
				Chunk * chunk = world.chunks[cx][cy][cz];
				if (chunk == NULL || chunk->isEmpty())
					continue;
				
				chunkCount++;
				chunk->toBytes(chunkBytes);
				totalUncompressed += CHUNK_BYTES;
				
				unsigned long rleLen;
				util::rleEncode(chunkBytes, CHUNK_BYTES, rleBytes, rleLen);
				totalCompressed += rleLen;
				
				chunkData cd = {new char[rleLen], rleLen};
				memcpy(cd.bytes, rleBytes, rleLen);
				chunkDatas.push_back(cd);
				
				chunkInfos.push_back({chunk->cx, chunk->cy, chunk->cz, static_cast<int>(rleLen)});
			}
	std::cout << "   Chunks: " << chunkCount << std::endl;
	std::cout << "   Uncompressed bytes: " << totalUncompressed << " (" << totalUncompressed/1048576.0f << " MB)" << std::endl;
	std::cout << "   Compressed bytes:   " << totalCompressed << " (" << totalCompressed/1048576.0f << " MB)" << std::endl;
	std::cout << "   (" << (float(totalCompressed)/totalUncompressed) * 100 << "% the original size)" << std::endl;
	
	// write data to file
	ortho::drawLoadingScreen("Saving world", "Writing file");
	std::cout << "Writing file..." << std::endl;
	std::ofstream fileOut(dataDir+"save.br", std::ios::binary | std::ios::out | std::ios::trunc);
	
	char buf[2];
	intToBytes(chunkCount, buf);
	fileOut.write(buf, 2);
	
	for (chunkInfo ci : chunkInfos) {
		intToBytes(ci.cx, buf);
		fileOut.write(buf, 2);
		intToBytes(ci.cy, buf);
		fileOut.write(buf, 2);
		intToBytes(ci.cz, buf);
		fileOut.write(buf, 2);
		
		intToBytes(ci.bytesLen, buf);
		fileOut.write(buf, 2);
	}
	
	for (chunkData cd : chunkDatas) {
		fileOut.write(cd.bytes, cd.len);
		delete [] cd.bytes;
	}
	
	fileOut.close();
	ortho::drawLoadingScreen("Saving world", "Done");
	std::cout << "Done." << std::endl << std::endl;//*/
}

void util::loadWorld() {
	ortho::drawLoadingScreen("Loading saved world", "Reading file");
	std::cout << "Loading world..." << std::endl;
	world.destruct(); // make sure existing resources are freed
	
	// read file
	std::cout << "Reading file..." << std::endl;
	std::ifstream fileIn(getDataDir()+"save.br", std::ios::binary | std::ios::in);
	
	// read header
	char buf[2];
	fileIn.read(buf, 2);
	int chunkCount;
	intFromBytes(chunkCount, buf);
	
	std::vector<chunkInfo> chunkInfos;
	for (int n = 0; n < chunkCount; n++) {
		chunkInfo ci;
		
		fileIn.read(buf, 2);
		intFromBytes(ci.cx, buf);
		fileIn.read(buf, 2);
		intFromBytes(ci.cy, buf);
		fileIn.read(buf, 2);
		intFromBytes(ci.cz, buf);
		
		fileIn.read(buf, 2);
		intFromBytes(ci.bytesLen, buf);
		
		chunkInfos.push_back(ci);
	}
	
	// read/decompress chunk data
	ortho::drawLoadingScreen("Loading saved world", "Decompressing data");
	std::cout << "Decompressing data..." << std::endl;
	char chunkBytes[CHUNK_BYTES+1];
	unsigned long chunkBytesLen;
	for (chunkInfo ci : chunkInfos) {
		// read data from file
		char * rleBytes = new char[ci.bytesLen];
		fileIn.read(rleBytes, ci.bytesLen);
		
		// RLE Decode
		rleDecode(rleBytes, ci.bytesLen, chunkBytes, chunkBytesLen, CHUNK_BYTES+1);
		delete [] rleBytes;
		if (chunkBytesLen != CHUNK_BYTES) {
			ortho::drawLoadingScreen("Loading saved world", "ERROR: Save file corrupt");
			std::cout << "ERROR: Decompressed data has invalid length (" << chunkBytesLen << ") - save corrupt" << std::endl << std::endl;
			return;
		}
		
		// add chunk to world
		Chunk * chunk = new Chunk(ci.cx, ci.cy, ci.cz, &world);
		chunk->fromBytes(chunkBytes);
		world.chunks[ci.cx][ci.cy][ci.cz] = chunk;
	}
	
	fileIn.close();
	ortho::drawLoadingScreen("Loading saved world", "Done");
	std::cout << "Done." << std::endl << std::endl;
}

bool util::fileExists(std::string path) {
	return std::ifstream(path).is_open();
}

void util::intToBytes(int i, char* dst) {
	unsigned char b1 = i % 256;
	unsigned char b2 = i / 256;
	dst[0] = b1;
	dst[1] = b2;
}

void util::intFromBytes(int& i, char* src) {
	i = (unsigned char)src[0];
	i += ((unsigned char)src[1]) * 256;
}

std::string util::getDataDir() {
	std::string homePath = getenv("HOME");
	return homePath + "/BedrockData/";
}

#ifdef win32

//// Windows ////
#error util::createDirectory not ported to Windows yet!
/////////////////

#else

///// Unix /////
#include <sys/types.h> // unused - what is this for?
#include <sys/stat.h>

bool util::createDirectory(std::string path) {
	return mkdir(path.c_str(), 0777) == 0;
}
////////////////

#endif

void util::rleEncode(char* data, unsigned long dataLen, char* res, unsigned long& resLen) {
	// Run-Length Encode the data
	resLen = 0;
	register int runLen = 0;
	char runOf = data[0];
	
	for (register int i = 0; i < dataLen; i++) {
		if (data[i] == runOf && runLen < CHAR_MAX)
			runLen++;
		else {
			res[resLen++] = runLen;
			res[resLen++] = runOf;
			runOf = data[i];
			runLen = 1;
		}
	}
	
	res[resLen++] = runLen;
	res[resLen++] = runOf;
}

void util::rleDecode(char* data, unsigned long dataLen, char* res, unsigned long& resLen, unsigned long limit) {
	// Run-Length Decode the data
	resLen = 0;
	int runLen = 0;
	char runOf;
	
	for (register unsigned long i = 0; i < dataLen; i++) {
		runLen = data[i];
		runOf = data[++i];
		if (resLen + runLen > limit) {
			runLen = int(limit - resLen);
			i = dataLen; // stop iterating
		}
		for (int n = 0; n < runLen; n++)
			res[resLen++] = runOf;
	}
}










