//
//  globals.h
//  Bedrock
//
//  Created by Quinn on 9/21/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "World.h"
#include "Player.h"
#include "Unit.h"

#include <thread>
#include <vector>


#define FoVy 60.0f
#define FOG_RADIUS 15.0f


extern World world;
extern bool worldSaved;
extern bool worldInitialized;
extern Player player;
extern Unit* curUnit;
extern std::vector<Unit*> units;

extern unsigned long terrainQuadsRendered;

extern int width, height;

// thread for background work, such as loading/saving the world while the GUI runs
/*extern std::thread workThread;
extern std::mutex workThreadRunning;
extern std::string workThreadTitle;
extern std::string workThreadStatus;
extern std::mutex workThreadStatus_mtx;*/



//// rendering stuff ////
#define QUADS // if defined, use quads, else use triangle fans
#ifdef QUADS
	#define POLYGON_TYPE GL_QUADS
	#define NEXT_POLYGON do{}while(false)
#else
	#define POLYGON_TYPE GL_TRIANGLE_FAN
	#define NEXT_POLYGON do{ glEnd(); glBegin(POLYGON_TYPE); }while(false)
#endif

//#define WORLD_VIEW // if defined, render whole world
#ifdef WORLD_VIEW
	#warning World view mode is ON
#endif