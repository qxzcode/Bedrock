//
//  main.cpp
//  Bedrock
//
//  Created by Quinn on 9/13/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#define ZNEAR 0.1f
#define ZFAR 1000.0f

#define FPS 60
#define	FR_L 1000/FPS
#define DT_CAP 20 // 50 FPS

#include <iostream>
#include "OpenGL.h"
#include </Library/Frameworks/SDL.framework/Headers/SDL.h>
#include <ctime>
#include "math.h"
#include <thread>

#include "textures.h"
#include "shaders.h"
#include "util.h"
#include "frustum.h"
#include "DisplayList.h"
#include "globals.h"
#include "input.h"
#include "worldGen.h"
#include "ortho.h"
#include "hud.h"
#include "MoveCommand.h"
#include "MineCommand.h"
#include "DroppedItem.h"


long lastFrame;
static long dt;

void initWorld();

GLfloat projectionMatrix[16]; // stores the projection matrix (for extracting the view frustum)
GLfloat viewMatrix[16]; // stores the view matrix (for shaders and unprojecting to world coords)
static bool viewMatrixInitialized = false;

static bool selectSolid = false;

// used for raytracing
static bool isBlockSolid(int x, int y, int z, util::vec3f face) {
	blockID block = world.getBlock(x, y, z);
	if (selectSolid)
		return Block::block(block)->isSolid;
	else
		return Block::block(block)->isSolid || block==TALL_GRASS_ID||block==FLOWER_GRASS_ID||block==MOUNTAIN_GRASS_ID;
}
static util::vec3f unproject(int mouse_x, int mouse_y);

#define FPS_LOG_LEN 30
static float fpsLog[FPS_LOG_LEN];	// stores previous FPS calculations
static int fpsLogPos = 0;			// to smooth out the value displayed
static unsigned long frameNum = 0;

static bool firstFrame = true;

static void displayScene();

/* glut display callback function.  Every time the window needs to be drawn,
 glut will call this function.  This includes when the window size
 changes, or when another window covering part of this window is
 moved so this window is uncovered.
 */
void display() {
	if (firstFrame)
		firstFrame = false;
	else {
		if (worldInitialized)
			displayScene();
		else {
			initWorld();
			worldInitialized = true;
		}
	}
}

extern bool bedrockRunning;

static void calcFrameVars() {
	frameNum++;
	/* calculate dt (delta time) */
	dt = SDL_GetTicks() - lastFrame;
	if (dt <= 0) dt = 1;
	lastFrame = SDL_GetTicks();
	// calc fps
	fpsLog[fpsLogPos++] = 1000.0f / dt;
	if (fpsLogPos >= FPS_LOG_LEN) fpsLogPos = 0;
	if (frameNum == 4) {
		// prevent "catching up" in first few frames
		float fps = fpsLog[fpsLogPos-1];
		for (int i = 0; i < FPS_LOG_LEN; i++)
			fpsLog[i] = fps;
	}
	if (dt > DT_CAP) dt = DT_CAP; // cap dt (so stuff doesn't malfunction during extreme lag)
}

Entity * entitySelection;
static util::vec3i blockSelection;
static bool isBlockSelected;
/*static*/ void doSelect() {
	if (!viewMatrixInitialized) {
		isBlockSelected = false;
		return;
	}
	
	util::vec3f origin = {player.dbg_x, player.dbg_y, player.dbg_z};
	util::vec3f dir = unproject(input::mouseX, input::mouseY);
	
	// BLOCKS
	blockSelection = util::rayTrace(origin, dir, 200, isBlockSolid);
	util::vec3f toUnit = util::vec3f{curUnit->x,curUnit->y,curUnit->z} - (util::vec3f{blockSelection.x,blockSelection.y,blockSelection.z}+0.5f);
	float dist = curUnit->viewDistance;
	if (toUnit.length() > dist)
		blockSelection = {-1, -1, -1};
	isBlockSelected = !(blockSelection.x==-1 && blockSelection.y==-1 && blockSelection.z==-1);
	
	// ENTITIES
	Entity * closest = NULL;
	float closestDist = 9001;
	for (Entity * e : world.entities) {
		using std::max;
		if ((util::vec3f{curUnit->x,curUnit->y,curUnit->z}-util::vec3f{e->x,e->y,e->z}).length()+(max(max(e->rx,e->ry),e->rz)*1.73205080757) > curUnit->viewDistance)
			continue;
		util::vec3f hit = util::rayToAABB({e->x,e->y,e->z,e->rx,e->ry,e->rz}, origin, dir);
		if (hit == util::vec3f{-1,-1,-1})
			continue;
		float dist = (origin-hit).length();
		if (dist < closestDist) {
			closest = e;
			closestDist = dist;
		}
	}
	
	// FINAL RESULT
	if (closest) {
		if (!isBlockSelected || ((util::vec3f(blockSelection)+0.5)-origin).length() > closestDist) {
			isBlockSelected = false;
			entitySelection = closest;
		} else entitySelection = NULL;
	} else entitySelection = NULL;
}

static util::vec3i* corner1 = NULL;
#define SELECT_FACE_OFFSET 0.01f
static void drawWireArea() {
	glUniform1i(shaders::fog_plainLoc, true);
	glColor3f(0, 0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	using util::vec3i; using util::vec3f;
#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)
	vec3i cMini = {min(blockSelection.x,corner1->x), min(blockSelection.y,corner1->y), min(blockSelection.z,corner1->z)};
	vec3i cMaxi = {max(blockSelection.x,corner1->x), max(blockSelection.y,corner1->y), max(blockSelection.z,corner1->z)};
#undef min
#undef max
	vec3f cMin = vec3f(cMini) - SELECT_FACE_OFFSET;
	vec3f cMax = vec3f(cMaxi) + SELECT_FACE_OFFSET;
	vec3f dim = (cMax-cMin) + 1;
	
	DisplayList dl;
	
	//// draw outline ////
	// outside
	glPushMatrix();
	{
		glTranslatef(cMin.x, cMin.y, cMin.z);
		
		dl.newList(true);
		glBegin(GL_LINE_LOOP);
		{
			glVertex3f(0,		0,		0);
			glVertex3f(dim.x,	0,		0);
			glVertex3f(dim.x,	0,		dim.z);
			glVertex3f(0,		0,		dim.z);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		{
			glVertex3f(0,		dim.y,	dim.z);
			glVertex3f(dim.x,	dim.y,	dim.z);
			glVertex3f(dim.x,	dim.y,	0);
			glVertex3f(0,		dim.y,	0);
		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex3f(0,		0,		0);
			glVertex3f(0,		dim.y,	0);
			glVertex3f(dim.x,	0,		0);
			glVertex3f(dim.x,	dim.y,	0);
			glVertex3f(dim.x,	0,		dim.z);
			glVertex3f(dim.x,	dim.y,	dim.z);
			glVertex3f(0,		0,		dim.z);
			glVertex3f(0,		dim.y,	dim.z);
		}
		glEnd();
		dl.endList();
	}
	glPopMatrix();
	
	// inside
	glPushMatrix();
	{
		glTranslatef(cMin.x, cMin.y, cMin.z);
		vec3f dimi = (cMaxi-cMini) + 1;
		vec3f scale = dimi/dim;
		glScalef(scale.x*scale.x, scale.y*scale.y, scale.z*scale.z);
		
		dl.call();
	}
	glPopMatrix();
	
	//// draw grid on faces ////
	// X
	dl.newList(true);
	{
		glBegin(GL_LINES);
		for (int y = cMini.y+1; y <= cMaxi.y; y++) {
			glVertex3f(cMini.x, y, cMini.z);
			glVertex3f(cMini.x, y, cMaxi.z+1);
			glVertex3f(cMin.x-SELECT_FACE_OFFSET, y, cMin.z-SELECT_FACE_OFFSET);
			glVertex3f(cMin.x-SELECT_FACE_OFFSET, y, cMax.z+1+SELECT_FACE_OFFSET);
		}
		for (int z = cMini.z+1; z <= cMaxi.z; z++) {
			glVertex3f(cMini.x, cMini.y, z);
			glVertex3f(cMini.x, cMaxi.y+1, z);
			glVertex3f(cMin.x-SELECT_FACE_OFFSET, cMin.y-SELECT_FACE_OFFSET, z);
			glVertex3f(cMin.x-SELECT_FACE_OFFSET, cMax.y+1+SELECT_FACE_OFFSET, z);
		}
		glEnd();
	}
	dl.endList();
	glPushMatrix();
	glTranslatef(dim.x, 0, 0);
	dl.call();
	glPopMatrix();
	
	// Y
	dl.newList(true);
	{
		glBegin(GL_LINES);
		for (int x = cMini.x+1; x <= cMaxi.x; x++) {
			glVertex3f(x, cMini.y, cMini.z);
			glVertex3f(x, cMini.y, cMaxi.z+1);
			glVertex3f(x, cMin.y-SELECT_FACE_OFFSET, cMin.z-SELECT_FACE_OFFSET);
			glVertex3f(x, cMin.y-SELECT_FACE_OFFSET, cMax.z+1+SELECT_FACE_OFFSET);
		}
		for (int z = cMini.z+1; z <= cMaxi.z; z++) {
			glVertex3f(cMini.x, cMini.y, z);
			glVertex3f(cMaxi.x+1, cMini.y, z);
			glVertex3f(cMin.x-SELECT_FACE_OFFSET, cMin.y-SELECT_FACE_OFFSET, z);
			glVertex3f(cMax.x+1+SELECT_FACE_OFFSET, cMin.y-SELECT_FACE_OFFSET, z);
		}
		glEnd();
	}
	dl.endList();
	glPushMatrix();
	glTranslatef(0, dim.y, 0);
	dl.call();
	glPopMatrix();
	
	// Z
	dl.newList(true);
	{
		glBegin(GL_LINES);
		for (int x = cMini.x+1; x <= cMaxi.x; x++) {
			glVertex3f(x, cMini.y, cMini.z);
			glVertex3f(x, cMaxi.y+1, cMini.z);
			glVertex3f(x, cMini.y-SELECT_FACE_OFFSET, cMini.z-SELECT_FACE_OFFSET);
			glVertex3f(x, cMaxi.y+1+SELECT_FACE_OFFSET, cMini.z-SELECT_FACE_OFFSET);
		}
		for (int y = cMini.y+1; y <= cMaxi.y; y++) {
			glVertex3f(cMini.x, y, cMini.z);
			glVertex3f(cMaxi.x+1, y, cMini.z);
			glVertex3f(cMin.x-SELECT_FACE_OFFSET, y, cMin.z-SELECT_FACE_OFFSET);
			glVertex3f(cMax.x+1+SELECT_FACE_OFFSET, y, cMin.z-SELECT_FACE_OFFSET);
		}
		glEnd();
	}
	dl.endList();
	glPushMatrix();
	glTranslatef(0, 0, dim.z);
	dl.call();
	glPopMatrix();
}
static void drawWireBlock() {
	if (corner1) {
		drawWireArea();
		return;
	}
	
	glUniform1i(shaders::fog_plainLoc, true);
	glColor3f(0, 0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_DEPTH_TEST);
	
	glPushMatrix();
	{
		glTranslatef(blockSelection.x, blockSelection.y, blockSelection.z);
		glBegin(GL_LINE_LOOP);
		{
			glVertex3f(0, 0, 0);
			glVertex3f(1, 0, 0);
			glVertex3f(1, 0, 1);
			glVertex3f(0, 0, 1);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		{
			glVertex3f(0, 1, 1);
			glVertex3f(1, 1, 1);
			glVertex3f(1, 1, 0);
			glVertex3f(0, 1, 0);
		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex3f(0, 0, 0);
			glVertex3f(0, 1, 0);
			glVertex3f(1, 0, 0);
			glVertex3f(1, 1, 0);
			glVertex3f(1, 0, 1);
			glVertex3f(1, 1, 1);
			glVertex3f(0, 0, 1);
			glVertex3f(0, 1, 1);
		}
		glEnd();
	}
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}
static void drawSelectArea(bool good) {
	glUniform1i(shaders::fog_plainLoc, true);
	glColor4f(1, good, good, 0.2); // if good is false, the cube is red
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_CULL_FACE);
	
	using util::vec3f;
#define min(a,b) static_cast<float>(a<b?a:b)
#define max(a,b) static_cast<float>(a>b?a:b)
	vec3f cMin = vec3f{min(blockSelection.x,corner1->x), min(blockSelection.y,corner1->y), min(blockSelection.z,corner1->z)} - SELECT_FACE_OFFSET;
	vec3f cMax = vec3f{max(blockSelection.x,corner1->x), max(blockSelection.y,corner1->y), max(blockSelection.z,corner1->z)} + SELECT_FACE_OFFSET;
#undef min
#undef max
	vec3f dim = (cMax-cMin) + 1;
	
	// used for face sorting
	bool xpFront = player.x > cMax.x,
		 xnFront = player.x < cMin.x,
		 ypFront = player.y > cMax.y,
		 ynFront = player.y < cMin.y,
		 zpFront = player.z > cMax.z,
		 znFront = player.z < cMin.z;
	
	glPushMatrix();
	{
		glTranslatef(cMin.x, cMin.y, cMin.z);
		glBegin(GL_QUADS);
		{
			// X+
#define XP	{glVertex3f(dim.x,	dim.y,	0);\
			glVertex3f(dim.x,	dim.y,	dim.z);\
			glVertex3f(dim.x,	0,		dim.z);\
			glVertex3f(dim.x,	0,		0);}
			
			// X-
#define XN	{glVertex3f(0,		0,		0);\
			glVertex3f(0,		0,		dim.z);\
			glVertex3f(0,		dim.y,	dim.z);\
			glVertex3f(0,		dim.y,	0);}
			
			// Y+
#define YP	{glVertex3f(0,		dim.y,	dim.z);\
			glVertex3f(dim.x,	dim.y,	dim.z);\
			glVertex3f(dim.x,	dim.y,	0);\
			glVertex3f(0,		dim.y,	0);}
			
			// Y-
#define YN	{glVertex3f(0,		0,		0);\
			glVertex3f(dim.x,	0,		0);\
			glVertex3f(dim.x,	0,		dim.z);\
			glVertex3f(0,		0,		dim.z);}
			
			// Z+
#define ZP	{glVertex3f(dim.x,	0,		dim.z);\
			glVertex3f(dim.x,	dim.y,	dim.z);\
			glVertex3f(0,		dim.y,	dim.z);\
			glVertex3f(0,		0,		dim.z);}
			
			// Z-
#define ZN	{glVertex3f(0,		0,		0);\
			glVertex3f(0,		dim.y,	0);\
			glVertex3f(dim.x,	dim.y,	0);\
			glVertex3f(dim.x,	0,		0);}
			
			// PASS 1
			if (!xpFront) XP;
			if (!xnFront) XN;
			if (!ypFront) YP;
			if (!ynFront) YN;
			if (!zpFront) ZP;
			if (!znFront) ZN;
			
			// PASS 2
			if (xpFront) XP;
			if (xnFront) XN;
			if (ypFront) YP;
			if (ynFront) YN;
			if (zpFront) ZP;
			if (znFront) ZN;
			
#undef XP
#undef XN
#undef YP
#undef YN
#undef ZP
#undef ZN
		}
		glEnd();
	}
	glPopMatrix();
	glEnable(GL_CULL_FACE);
}
static void drawSelectBlock(bool good) {
	if (corner1) {
		drawSelectArea(good);
		return;
	}
	
	glUniform1i(shaders::fog_plainLoc, true);
	glColor4f(1, good, good, 0.2); // if good is false, the cube is red
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_DEPTH_TEST);
	
	glPushMatrix();
	{
		glTranslatef(blockSelection.x, blockSelection.y, blockSelection.z);
		glBegin(GL_QUADS);
		{
			// bottom
			glVertex3f(0, 0, 0);
			glVertex3f(1, 0, 0);
			glVertex3f(1, 0, 1);
			glVertex3f(0, 0, 1);
			
			// top
			glVertex3f(0, 1, 1);
			glVertex3f(1, 1, 1);
			glVertex3f(1, 1, 0);
			glVertex3f(0, 1, 0);
			
			// left
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 1);
			glVertex3f(0, 1, 1);
			glVertex3f(0, 1, 0);
			
			// right
			glVertex3f(1, 1, 0);
			glVertex3f(1, 1, 1);
			glVertex3f(1, 0, 1);
			glVertex3f(1, 0, 0);
			
			// back
			glVertex3f(0, 0, 0);
			glVertex3f(0, 1, 0);
			glVertex3f(1, 1, 0);
			glVertex3f(1, 0, 0);
			
			// front
			glVertex3f(1, 0, 1);
			glVertex3f(1, 1, 1);
			glVertex3f(0, 1, 1);
			glVertex3f(0, 0, 1);
		}
		glEnd();
	}
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}
static int selecting = 0; // 0 for no cube, 1 for white, 2 for red

static bool inHUD = false; // true if the mouse is over the HUD







/* UPDATE WORLD/ENTITIES */
void update() {
	calcFrameVars();
	doSelect();
	
	// handle command block selection
	using hud::CommandEnum;
	switch (hud::issuingCmd) {
		case CommandEnum::MOVE:
			selecting = 1;
			if (input::mouseLeftClicked && isBlockSelected && !inHUD) {
				input::mouseLeftClicked = false;
				curUnit->cmdQueue.push(
									   new MoveCommand({static_cast<float>(blockSelection.x)+0.5f, static_cast<float>(blockSelection.y)+0.5f, static_cast<float>(blockSelection.z)+0.5f})
									   );
				selecting = 0;
				hud::issuingCmd = CommandEnum::NONE;
			}
			break;
			
		case CommandEnum::MINE:
			selecting = 1;
			if (input::mouseLeftClicked && isBlockSelected && !inHUD) {
				input::mouseLeftClicked = false;
				if (corner1) {
					curUnit->cmdQueue.push(new MineCommand(*corner1, blockSelection));
					
					delete corner1; corner1 = NULL;
					selecting = 0;
					hud::issuingCmd = CommandEnum::NONE;
				} else {
					corner1 = new util::vec3i(blockSelection);
				}
			}
			break;
			
			
		default: selecting = 0; delete corner1; corner1 = NULL; break;
	}
	if (hud::issuingCmd == CommandEnum::NONE &&
		player.controllingUnit &&
		input::mouseLeftClicked &&
		entitySelection&&entitySelection->type()==EntityType::DROPPED_ITEM) {
		if (!curUnit->leftItem) {
			curUnit->leftItem = static_cast<DroppedItem*>(entitySelection)->item;
			entitySelection->remove = true;
			entitySelection = NULL;
		} else if (!curUnit->rightItem) {
			curUnit->rightItem = static_cast<DroppedItem*>(entitySelection)->item;
			entitySelection->remove = true;
			entitySelection = NULL;
		}
	}
	
	world.update(dt);
	player.update(dt);
	for (Unit* unit : units)
		unit->update(dt);
	if (player.controllingUnit)
		player.set1stPersonPos();
	// save world & quit if ESC is pressed
	if (input::quit) {
		util::saveWorld();
		worldSaved = true;
		// exit the game
		bedrockRunning = false;
		return;
	}
}


/* DRAW SCENE */
static void displayScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear the screen
	
	// SHADER STUFF
	{
		glUniform3f(shaders::fog_fogCenterLoc, curUnit->x, curUnit->y, curUnit->z);
		glUniform1f(shaders::fog_fogEndLoc, curUnit->viewDistance);
		glUniform1i(shaders::fog_fogOnLoc, input::fog);
/*#warning TESTING
		if (frameNum == 5)*/
		glUniform1f(shaders::fog_grassWaveLoc, lastFrame/350.0);
		glUniform1i(shaders::fog_plainLoc, false);
		glUniform1i(shaders::fog_selectLoc, false);
		
		glLoadIdentity();		// load the identity matrix (reset transformations)
		player.lookThrough();	// transform to player's PoV
		
		glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
		viewMatrixInitialized = true;
		frustumUtil::extractFrustum();
		glUniformMatrix4fv(shaders::fog_viewMatrixLoc, 1, false, viewMatrix);
		shaders::plain.useProgram();
		glUniformMatrix4fv(shaders::plain_viewMatrixLoc, 1, false, viewMatrix);
		shaders::fog.useProgram();
		
		glLoadIdentity();		// load the identity matrix (reset transformations)
	}
	
	world.draw();
#define max(a,b) (a>b?a:b)
	for (Unit* unit : units)
		if (frustumUtil::isCubeInFrustum(unit->x, unit->y, unit->z, max(max(unit->rx,unit->ry),unit->rz)))
			unit->draw();
	
#undef max
	player.draw();
	
#ifndef WORLD_VIEW
	// draw wireframe around selected block
	if (isBlockSelected && !inHUD) {
		if (selecting)
			drawSelectBlock(selecting == 1);
		drawWireBlock();
	}
	
	/* DRAW HUD */
	float fpsSum = 0;
	for (int i = 0; i < FPS_LOG_LEN; i++)
		fpsSum += fpsLog[i];
	
	std::string selection = isBlockSelected? Block::block(world.getBlock(blockSelection.x, blockSelection.y, blockSelection.z))->name :
							entitySelection? entitySelection->name() :
							"";
	inHUD = hud::draw(selection, (int)(fpsSum/FPS_LOG_LEN));
#endif
	
	/* swap the back and front buffers so we can see what we just drew */
	util::swapBuffers();
	
	// check for OpenGL errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) printf("OpenGL ERROR: 0x%X\n", error);
}







static util::mat4f projection_matrix_inv;

static util::vec3f unproject(int mouse_x, int mouse_y) {
	using util::vec3f; using util::vec4f; using util::mat4f;
	mat4f view_matrix_inv = mat4f(viewMatrix).inverse();
	
	float x = (2.0f * mouse_x) / width - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / height;
	vec3f ray_nds = {x, y, 1.0}; // normalized device coords
	vec4f ray_clip = {ray_nds.x, ray_nds.y, -1.0, 1.0}; // homogeneous clip coords
	vec4f ray_eye = projection_matrix_inv * ray_clip; // 4D camera coords
	ray_eye = {ray_eye.x, ray_eye.y, -1.0, 0.0};
	vec3f ray_wor = (view_matrix_inv * ray_eye).xyz(); // world coords (relative to camera)
	if (ray_wor.x == 0 && ray_wor.y == 0 && ray_wor.z == 0)
		return {0, 0, 1};
	return ray_wor;//.normalize();
}

/* 
 * this is called when the window changes size, and when it is created
 */
void reshape(int w, int h) {
	width = w; height = h;
	
	/* tell OpenGL we want to display in a recangle that is the
	 same size as the window
	 */
	glViewport(0,0,width,height);
	
	/* switch to the projection matrix */
	glMatrixMode(GL_PROJECTION);
	
	/* clear the projection matrix */
	glLoadIdentity();
	
	// Calculate The Aspect Ratio Of The Window
    gluPerspective(FoVy,(GLfloat)width/(GLfloat)height,ZNEAR,ZFAR);
	
	/* switch back to the model view matrix */
	glMatrixMode(GL_MODELVIEW);
	
	// cache the projection matrix and its inverse
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
	projection_matrix_inv = util::mat4f(projectionMatrix).inverse();
	
	// redraw HUD
	hud::hudBG.deleteList();
}
