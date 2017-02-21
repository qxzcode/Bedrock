//
//  Player.cpp
//  Bedrock
//
//  Created by Quinn on 9/29/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "Player.h"

#include "input.h"
#include "util.h"
#include "globals.h"

#include "math.h"
#include "DroppedItem.h"

Player::Player(float x, float y, float z, World * w):Entity(x,y,z,w,PLAYER_SIZE),dbg_x(x),dbg_y(y),dbg_z(z) {
	
}

void Player::set1stPersonPos() {
	this->dbg_x = curUnit->x;
	this->dbg_y = curUnit->y+0.67; // eye level
	this->dbg_z = curUnit->z;
}

void Player::update(long dt) {
	if (controllingUnit) {
		
		
		//set1stPersonPos();
		
		//// Keyboard movement \\\\
		
		if (input::playerForwards)
			curUnit->accelerate();
		if (input::playerBackwards)
			curUnit->decelerate();
		if (input::playerUp)
			curUnit->jump();
		
		// misc. keyboard
		if (input::dropItem) {
			input::dropItem = false;
			Item* & item = activeArm? curUnit->leftItem : curUnit->rightItem;
			if (item) {
				DroppedItem* e = new DroppedItem(x,y,z,world,item);
				e->ySpeed = curUnit->ySpeed + 7+(pitch*(20.0/90));
				e->xSpeed = curUnit->xSpeed + sinf(-(yaw+180) * DEG_TO_RAD)*25;
				e->zSpeed = curUnit->zSpeed + cosf(-(yaw+180) * DEG_TO_RAD)*25;
				world->addEntity(e);
				item = NULL;
			}
		}
		if (input::switchArms) {
			input::switchArms = false;
			activeArm = !activeArm;
		}
		
		//// Mouse movement \\\\
		
		dbg_yaw += input::mouseDragDX / 2.0f;
		dbg_pitch -= input::mouseDragDY / 2.0f;
		input::mouseDragDX = input::mouseDragDY = 0;
		// constrain values
		if (dbg_pitch > 90) dbg_pitch = 90;
		if (dbg_pitch < -90) dbg_pitch = -90;
		
		curUnit->yaw = this->dbg_yaw + 180;
		
		
	} else { // not controlling unit
		input::dropItem = false;
		input::switchArms = false;
		
		//// Keyboard movement \\\\
		
		float dLR = 0, dFB = 0;
		
		if (input::playerForwards)
			dFB -= dt*moveSpeed;
		if (input::playerBackwards)
			dFB += dt*moveSpeed;
		if (input::playerLeft)
			dLR -= dt*moveSpeed;
		if (input::playerRight)
			dLR += dt*moveSpeed;
		
		if (input::playerUp)
			dbg_y += dt*moveSpeed;
		if (input::playerDown)
			dbg_y -= dt*moveSpeed;
		
		// reduce diagonal movement
		if (dLR && dFB) {
			dLR /= SQRT_2;
			dFB /= SQRT_2;
		}
		
		dbg_z += cosf(-dbg_yaw * DEG_TO_RAD) * dFB;
		dbg_x += sinf(-dbg_yaw * DEG_TO_RAD) * dFB;
		dbg_z += cosf(-(dbg_yaw-90) * DEG_TO_RAD) * dLR;
		dbg_x += sinf(-(dbg_yaw-90) * DEG_TO_RAD) * dLR;
		
#ifndef WORLD_VIEW
		// if unit has wrapped, wrap the camera as well
		if (curUnit->x - lastTX > WORLD_X/2) dbg_x += WORLD_X;
		if (curUnit->x - lastTX < -WORLD_X/2) dbg_x -= WORLD_X;
		if (curUnit->y - lastTY > WORLD_Y/2) dbg_y += WORLD_Y;
		if (curUnit->y - lastTY < -WORLD_Y/2) dbg_y -= WORLD_Y;
		if (curUnit->z - lastTZ > WORLD_Z/2) dbg_z += WORLD_Z;
		if (curUnit->z - lastTZ < -WORLD_Z/2) dbg_z -= WORLD_Z;
		
		// stay inside unit's viewDistance
		using util::vec3f;
		vec3f unitPos = {curUnit->x, curUnit->y, curUnit->z}, toUnit = vec3f{dbg_x, dbg_y, dbg_z} - unitPos;
		if (toUnit.length() > curUnit->viewDistance) {
			toUnit = (toUnit.normalize() * curUnit->viewDistance) + unitPos;
			dbg_x = toUnit.x; dbg_y = toUnit.y; dbg_z = toUnit.z;
		}
#endif
		
		lastTX = curUnit->x;
		lastTY = curUnit->y;
		lastTZ = curUnit->z;
		
		// collisions
		{
			float tmp_x = x, tmp_y = y, tmp_z = z;
			x = dbg_x; y = dbg_y; z = dbg_z;
			doCollide(); // perform collision detection/response
			dbg_x = x; dbg_y = y; dbg_z = z;
			x = tmp_x; y = tmp_y; z = tmp_z;
		}
		
		//// Mouse movement \\\\
		
		dbg_yaw += input::mouseDragDX / 2.0f;
		dbg_pitch -= input::mouseDragDY / 2.0f;
		input::mouseDragDX = input::mouseDragDY = 0;
		// constrain values
		if (dbg_pitch > 90) dbg_pitch = 90;
		if (dbg_pitch < -90) dbg_pitch = -90;
		//dbg_yaw = util::mod(dbg_yaw+360, 360);
		if (fabsf(modff(dbg_y, &dFB)) == 0) dbg_y += 0.0001;
		
		
	}
	
	if (!input::debug) {
		yaw = dbg_yaw;
		pitch = dbg_pitch;
		x = dbg_x;
		y = dbg_y;
		z = dbg_z;
	}
	
	//printf("Player pos=(%f, %f, %f), dbg_pos=(%f, %f, %f), pitch=%i°, yaw=%i°\n", x,y,z, dbg_x,dbg_y,dbg_z, (int)pitch, (int)yaw);
}

void Player::draw() {
	/*glPushMatrix();
	{
		glTranslatef(x, y, z);
		glBindTexture(GL_TEXTURE_2D, 0);
		glColor3f(0, 1, 0);
		glBegin(GL_LINE_LOOP);
		{
			glVertex3f(-rx, -ry, -rz);
			glVertex3f(-rx, -ry, rz);
			glVertex3f(rx, -ry, rz);
			glVertex3f(rx, -ry, -rz);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		{
			glVertex3f(-rx, ry, -rz);
			glVertex3f(-rx, ry, rz);
			glVertex3f(rx, ry, rz);
			glVertex3f(rx, ry, -rz);
		}
		glEnd();
		glBegin(GL_LINES);
		{
			glVertex3f(-rx, -ry, -rz);
			glVertex3f(-rx, ry, -rz);
			glVertex3f(-rx, -ry, rz);
			glVertex3f(-rx, ry, rz);
			glVertex3f(rx, -ry, rz);
			glVertex3f(rx, ry, rz);
			glVertex3f(rx, -ry, -rz);
			glVertex3f(rx, ry, -rz);
		}
		glEnd();
		glColor3f(1, 1, 1);
	}
	glPopMatrix();*/
}

void Player::lookThrough() {
	glRotatef(-dbg_pitch, 1, 0, 0);
	glRotatef(dbg_yaw, 0, 1, 0);
	glTranslatef(-dbg_x, -dbg_y, -dbg_z);
}
