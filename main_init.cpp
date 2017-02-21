//
//  main_init.cpp
//  Bedrock
//
//  Created by Quinn on 1/12/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include <iostream>
using std::cout; using std::endl;
#include "OpenGL.h"
#include </Library/Frameworks/SDL.framework/Headers/SDL.h>

#include "util.h"
#include "ortho.h"
#include "worldGen.h"
#include "globals.h"
#include "shaders.h"
#include "input.h"
#include "hud.h"

#include "MoveCommand.h"
#include "FlowerGrass.h"

#define WINDOW_TITLE "Bedrock"
#define WINDOW_SIZE 800, 600
#define WINDOW_POS 200, 150
#define BG_COLOR 0.8f, 0.8f, 0.8f, 1.0f // also fog color
//#define ANTIALIASING

/* intitialization functions */
static bool init();
static bool initSDL();
static bool initOpenGL();
/* update & display callback functions */
void update();
void display();
/* window reshape callback function */
void reshape(int, int);

static inline SDL_Surface* createWindow(int width, int height) {
	return SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL);
}
static SDL_Surface* displaySuface;

bool bedrockRunning = true;

int main(int argc, char** argv) {
	// initialize the game
	if (init() == false) return -1; // an error occurred
	
	/////// MAIN LOOP ///////
	SDL_Event event;
	while (bedrockRunning) {
		if (worldInitialized) update();
		display();
		input::mouseLeftClicked = false;
		
		// handle input events
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					util::saveWorld();
					worldSaved = true;
					// exit the game
					bedrockRunning = false;
					break;
					
				case SDL_VIDEORESIZE:
					// mark chunks for redrawing
					Chunk* chunk;
					for (int cx = 0; cx < WORLD_CX; cx++)
						for (int cy = 0; cy < WORLD_CX; cy++)
							for (int cz = 0; cz < WORLD_CX; cz++) {
								if ((chunk = world.chunks[cx][cy][cz]) == NULL)
									continue;
								chunk->needsRedraw = true;
							}
					world.firstDraw = true;
					displaySuface = createWindow(event.resize.w, event.resize.h); // create new window
					reshape(event.resize.w, event.resize.h); // reshape viewport & stuff
					initOpenGL(); // reload textures, shaders, etc.
					if (world.firstDraw)
						ortho::drawLoadingScreen("Entering world", "");
					break;
					
				default:
					input::handleEvent(event);
			}
		}
	}
	/////////////////////////
	
	// exit the program cleanly
	SDL_Quit();
	return 0;
}

#include "Bear.h"
#include "Rock.h"

void initWorld() {
	ortho::drawLoadingScreen("Initializing world", "");
	
	// initialize world
	if (util::fileExists(util::getDataDir()+"save.br")) {
		// load world from file
		util::loadWorld();
	} else {
		// generate a new world
		worldGen::generateWorld(world, (unsigned int)time(NULL));
	}
	
	/*
	 printf("COUNTING BLOCKS...\n");
	 int counts[NUM_BLOCKS];
	 for (int i = 0; i < NUM_BLOCKS; i++) counts[i] = 0;
	 for (int x = 0; x < WORLD_X; x++)
	 for (int y = 0; y < WORLD_X; y++)
	 for (int z = 0; z < WORLD_X; z++)
	 counts[world.getBlock(x, y, z)]++;
	 float total = WORLD_X*WORLD_Y*WORLD_Z;
	 for (int b = 0; b <= 7; b++) printf("%s (%i):\t%i\t%%%f\n", Block::block(b)->name.c_str(), b, counts[b], counts[b]/total*100);
	 printf("\n");
	 //*/
	
	ortho::drawLoadingScreen("Positioning units", "");
	
	// set unit positions (center of world, on ground)
	float groupX = random()%(WORLD_X*3/4) + (WORLD_X*1/8),
		  groupZ = random()%(WORLD_Z*3/4) + (WORLD_Z*1/8);
#define GROUP_SPREAD 20
#define RAND_SPREAD ((random()%GROUP_SPREAD)-(GROUP_SPREAD/2.0f))
#define NUM_UNITS 2
	for (int n = 0; n < NUM_UNITS; n++) {
		float unitX = groupX+RAND_SPREAD + 0.5,
			  unitZ = groupZ+RAND_SPREAD + 0.5,
			  unitY = 0;
		for (int y = WORLD_Y; y >= 0; y--) {
			if (Block::block(world.getBlock(unitX, y, unitZ))->isSolid) {
				unitY = y + 3;
				break;
			}
		}
		Unit * unit = new Unit(unitX, unitY, unitZ, &world);
		unit->yaw = random()%360;
		switch (n) {
			case 0: unit->occupations.insert({Unit::Occupation::librarian, 1}); break;
			case 1: unit->occupations.insert({Unit::Occupation::miner, 1}); break;
		}
		units.push_back(unit);
	}
	curUnit = units[0];
	player.dbg_x = player.x = player.lastX = player.lastTX = curUnit->x;
	player.dbg_y = player.y = player.lastY = player.lastTY = curUnit->y + 3;
	player.dbg_z = player.z = player.lastZ = player.lastTZ = curUnit->z - 3;
	/*player.x = player.lastX = player.lastTX = curUnit->x;
	player.z = player.lastZ = player.lastTY = curUnit->y+3;
	player.y = player.lastY = player.lastTZ = curUnit->z-3;*/
	player.dbg_yaw = player.yaw = 180;
	player.dbg_pitch = player.pitch = -30;
	
	// build menus for current unit
	hud::buildMenus();
	
#ifdef WORLD_VIEW
	player.y = player.lastY += 50;
	player.yaw -= 45;
	player.pitch += 10;
	player.x = player.lastX = -100;
	player.z = player.lastZ = -100;
#endif
	
	ortho::drawLoadingScreen("Entering world", "");
	
	
	
	
	world.addEntity(new Bear(curUnit->x+3,curUnit->y+10,curUnit->z+3,&world));
	curUnit->rightItem = new Rock();
	curUnit->leftItem = new Rock();
}

extern long lastFrame;

static bool init() {
	// initialize SDL & OpenGL states
	if (initSDL() == false ||
		initOpenGL() == false)
		return false; // an error occurred
	
	// initialize block list
	Block::initBlocks();
	
	lastFrame = SDL_GetTicks();
	
	// success!
	return true;
}

//// INIT SDL STUFFS ////
static bool initSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Failed to initialize SDL." << endl;
		return false;
	}
	
#ifdef ANTIALIASING
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
#endif
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, false); // don't limit FPS
	
	if ((displaySuface = createWindow(WINDOW_SIZE)) == NULL) {
		cout << "Failed to create SDL surface." << endl;
		return false;
	}
	reshape(WINDOW_SIZE);
	
	// success!
	return true;
}

//// INIT OpenGL STUFFS ////
static bool initOpenGL() {
	glEnable(GL_TEXTURE_2D);		// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);		// Enable Smooth Shading
	glClearColor(BG_COLOR);			// Background Color
	glClearDepth(1.0f);				// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);			// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Nice Perspective Correction
	glEnable(GL_TEXTURE_2D);		// Enable Textures
	glEnable(GL_CULL_FACE);			// Enable Backface Culling
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Blend Function
    glEnable(GL_BLEND);			// Enable blending (transparency)
	
	// initialize textures, and shaders
	textures::load();
	shaders::load();
	
	// initialize shader variables
	{
		shaders::fog.useProgram();
		
		GLint fogRadius = glGetUniformLocation(shaders::fog.progObj, "fogRadius");
		glUniform1f(fogRadius, FOG_RADIUS);
		
		GLint fogColor = glGetUniformLocation(shaders::fog.progObj, "fogColor");
		glUniform4f(fogColor, BG_COLOR);
		
		srandom((unsigned)time(NULL));
		static const int color = random() % FlowerGrass::numColors;
		util::vec3f petals = FlowerGrass::petalColors[color];
		glUniform3f(shaders::fog_flowerPetalsLoc, petals.x, petals.y, petals.z);
		util::vec3f center = FlowerGrass::centerColors[color];
		glUniform3f(shaders::fog_flowerCenterLoc, center.x, center.y, center.z);
		glUniform1i(shaders::fog_texturesLoc, true);
	}
	
	// success!
	return true;
}
