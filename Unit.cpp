//
//  Unit.cpp
//  Bedrock
//
//  Created by Quinn on 11/3/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "Unit.h"

#include "util.h"
#include "input.h"
#include "DroppedItem.h"

extern const float unitTexCoords[192];

#define BODY_RADIUS	6.0f*(0.5f/8.0f)
#define HEAD_RADIUS	4.0f*(0.5f/8.0f)
#define ARM_RADIUS	2.5f*(0.5f/8.0f)
#define LEG_RADIUS	2.5f*(0.5f/8.0f)

Unit::Unit(float x, float y, float z, World * w):Quadruped(x,y,z,w,UNIT_SIZE,100) {
	moveSpeed = 8.0f;
	acceleration = 1.5f;
	
	texture = textures::unit;
	texCoords = unitTexCoords;
	bodyDim = util::vec3f{BODY_RADIUS,BODY_RADIUS,BODY_RADIUS}*2;
	headDim = util::vec3f{HEAD_RADIUS,HEAD_RADIUS,HEAD_RADIUS}*2;
	armDim = util::vec3f{ARM_RADIUS,ARM_RADIUS,ARM_RADIUS}*2;
	legDim = util::vec3f{LEG_RADIUS,LEG_RADIUS,LEG_RADIUS}*2;
	bodyOffset = {0,0,0};
	headOffset = {0,BODY_RADIUS+HEAD_RADIUS,0};
	armOffset = {BODY_RADIUS+ARM_RADIUS,0,0};
	legOffset = {0.2f,-BODY_RADIUS-LEG_RADIUS,0};
}

struct direction {
	int x, z;
};

static inline direction getDirection(int yaw) {
	yaw = ((yaw%360)+360+45) % 360;
	if (yaw < 90) return {0, 1};
	if (yaw < 180) return {-1, 0};
	if (yaw < 270) return {0, -1};
	return {1, 0};
}

#define being_controlled (player.controllingUnit && curUnit == this)

void Unit::update(long dt) {
	
	if (!being_controlled) {
		//// basic movement ////
		
		// jump when running into a wall
		if ((lastCResult.x || lastCResult.z) && lastCResult.y==-1) {
			jump();
		}
		
		//// handle commands ////
		if (cmdQueue.empty()) {
			// no commands - idle
			
		} else {
			// execute current Command
			float dt_f = dt/1000.0f;
			if (cmdQueue.front()->doCommand(this, dt_f)) { // if (current command is finished) {
				delete cmdQueue.front(); // delete Command object
				cmdQueue.pop(); // remove the Command from the queue
				
				// some sort of GUI alert??
			}
		}
	}
	
	if (leftItem) leftItem->update(dt);
	if (rightItem) rightItem->update(dt);
	
	Quadruped::update(dt);
	
	/*/////////
	direction dir = getDirection(yaw);
	world->setBlockWithNotify(x + (dir.x*-2), y, z + (dir.z*-2), CLAY_ID);
#define SIZE 3
	if (random()%20 == 0)
		for (int x = this->x-SIZE; x < this->x+SIZE; x++)
			for (int y = this->y-SIZE; y < this->y+SIZE; y++)
				for (int z = this->z-SIZE; z < this->z+SIZE; z++)
					world->setBlockWithNotify(x, y, z, 0);
#undef SIZE
	/////////*/
}

void Unit::draw() {
	if (!being_controlled) {
		Quadruped::draw();
	} else {
		// draw just the arms
		glPushMatrix();
		{
			glTranslatef(x, y, z);
			glRotatef(-yaw, 0, 1, 0);
			glTranslatef(-armOffset.x,-armOffset.y,-armOffset.z);
			glTranslatef(0, +0.67, 0);
			glRotatef(-player.dbg_pitch/2.0f, 1, 0, 0);
			glTranslatef(0, -0.67, 0);
			glTranslatef(armOffset.x,armOffset.y,armOffset.z);
			glBindTexture(GL_TEXTURE_2D, texture);
			
			float anim = sinf(Quadruped::anim/200.0f)/20.0f;
			
			//// ARMS ////
#define ARM_SCALE 0.45
			glPushMatrix();
			{
				glTranslatef(-0.07, 0.5, anim + 0.3);
				glScalef(ARM_SCALE,ARM_SCALE,ARM_SCALE);
				drawArm(true);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslatef(0.07, 0.5, -anim + 0.3);
				glScalef(-ARM_SCALE,ARM_SCALE,ARM_SCALE);
				glDisable(GL_CULL_FACE);
				drawArm(false);
				glEnable(GL_CULL_FACE);
			}
			glPopMatrix();
		}
		glPopMatrix();
		glColor3f(1, 1, 1); // reset color (just in case)
	}
}

void Unit::drawArm(bool left) {
	if (player.controllingUnit&&curUnit==this&&left!=player.activeArm)
		return; // when in 1st person, only draw active arm
	Quadruped::drawArm(left);
#define PUSH_ATTRIBS glPushAttrib(GL_TEXTURE_BIT|GL_CURRENT_BIT);
#define POP_ATTRIBS  glPopAttrib();
	if (left) {
		if (leftItem) {
			PUSH_ATTRIBS
			leftItem->draw(armOffset.x, armOffset.y, armOffset.z, HELD_LEFT);
			POP_ATTRIBS
		}
	} else {
		if (rightItem) {
			PUSH_ATTRIBS
			glPushMatrix();
			glScalef(-1, 1, 1);
			rightItem->draw(-armOffset.x, armOffset.y, armOffset.z, HELD_RIGHT);
			glPopMatrix();
			POP_ATTRIBS
		}
	}
}

//// texcoord calculations ////

#define BODY_WIDTH 6.0f

#define TEXCOORDS1_BODY(n) n/BODY_WIDTH,BODY_WIDTH/UNIT_SHEET_HEIGHT
#define TEXCOORDS2_BODY(n) n/BODY_WIDTH,0.0f
#define TEXCOORDS3_BODY(n) (n-1)/BODY_WIDTH,0.0f
#define TEXCOORDS4_BODY(n) (n-1)/BODY_WIDTH,BODY_WIDTH/UNIT_SHEET_HEIGHT

#define HEAD_WIDTH 5.0f

#define TEXCOORDS1_HEAD(n) n/(UNIT_SHEET_WIDTH/HEAD_WIDTH),(BODY_WIDTH+HEAD_WIDTH)/UNIT_SHEET_HEIGHT
#define TEXCOORDS2_HEAD(n) n/(UNIT_SHEET_WIDTH/HEAD_WIDTH),BODY_WIDTH/UNIT_SHEET_HEIGHT
#define TEXCOORDS3_HEAD(n) (n-1)/(UNIT_SHEET_WIDTH/HEAD_WIDTH),BODY_WIDTH/UNIT_SHEET_HEIGHT
#define TEXCOORDS4_HEAD(n) (n-1)/(UNIT_SHEET_WIDTH/HEAD_WIDTH),(BODY_WIDTH+HEAD_WIDTH)/UNIT_SHEET_HEIGHT

#define ARM_WIDTH 2.0f

#define TEXCOORDS1_ARM(n) n/(UNIT_SHEET_WIDTH/ARM_WIDTH),(BODY_WIDTH+HEAD_WIDTH+ARM_WIDTH)/UNIT_SHEET_HEIGHT
#define TEXCOORDS2_ARM(n) n/(UNIT_SHEET_WIDTH/ARM_WIDTH),(BODY_WIDTH+HEAD_WIDTH)/UNIT_SHEET_HEIGHT
#define TEXCOORDS3_ARM(n) (n-1)/(UNIT_SHEET_WIDTH/ARM_WIDTH),(BODY_WIDTH+HEAD_WIDTH)/UNIT_SHEET_HEIGHT
#define TEXCOORDS4_ARM(n) (n-1)/(UNIT_SHEET_WIDTH/ARM_WIDTH),(BODY_WIDTH+HEAD_WIDTH+ARM_WIDTH)/UNIT_SHEET_HEIGHT

#define LEG_WIDTH 2.0f

#define TEXCOORDS1_LEG(n) n/(UNIT_SHEET_WIDTH/LEG_WIDTH),(BODY_WIDTH+HEAD_WIDTH+ARM_WIDTH+LEG_WIDTH)/UNIT_SHEET_HEIGHT
#define TEXCOORDS2_LEG(n) n/(UNIT_SHEET_WIDTH/LEG_WIDTH),(BODY_WIDTH+HEAD_WIDTH+ARM_WIDTH)/UNIT_SHEET_HEIGHT
#define TEXCOORDS3_LEG(n) (n-1)/(UNIT_SHEET_WIDTH/LEG_WIDTH),(BODY_WIDTH+HEAD_WIDTH+ARM_WIDTH)/UNIT_SHEET_HEIGHT
#define TEXCOORDS4_LEG(n) (n-1)/(UNIT_SHEET_WIDTH/LEG_WIDTH),(BODY_WIDTH+HEAD_WIDTH+ARM_WIDTH+LEG_WIDTH)/UNIT_SHEET_HEIGHT

#define TEXCOORDS_BODY(n) TEXCOORDS1_BODY(n),TEXCOORDS2_BODY(n),TEXCOORDS3_BODY(n),TEXCOORDS4_BODY(n)
#define TEXCOORDS_HEAD(n) TEXCOORDS1_HEAD(n),TEXCOORDS2_HEAD(n),TEXCOORDS3_HEAD(n),TEXCOORDS4_HEAD(n)
#define TEXCOORDS_ARM(n) TEXCOORDS1_ARM(n),TEXCOORDS2_ARM(n),TEXCOORDS3_ARM(n),TEXCOORDS4_ARM(n)
#define TEXCOORDS_LEG(n) TEXCOORDS1_LEG(n),TEXCOORDS2_LEG(n),TEXCOORDS3_LEG(n),TEXCOORDS4_LEG(n)
#define TEXCOORDS_PART(FUNC) FUNC(1),FUNC(2),FUNC(3),FUNC(4),FUNC(5),FUNC(6)
const float unitTexCoords[] = {TEXCOORDS_PART(TEXCOORDS_BODY),TEXCOORDS_PART(TEXCOORDS_HEAD),TEXCOORDS_PART(TEXCOORDS_ARM),TEXCOORDS_PART(TEXCOORDS_LEG)};
