//
//  globals.cpp
//  Bedrock
//
//  Created by Quinn on 9/21/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "globals.h"

World world;
bool worldSaved = false;
bool worldInitialized = false;
Player player(0, 0, 0, &world);
Unit* curUnit = NULL;
std::vector<Unit*> units;

unsigned long terrainQuadsRendered = 0;

int width, height;

/*std::thread workThread;
std::mutex workThreadRunning;
std::string workThreadTitle;
std::string workThreadStatus;
std::mutex workThreadStatus_mtx;*/