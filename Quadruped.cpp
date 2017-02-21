//
//  Quadruped.cpp
//  Bedrock
//
//  Created by Quinn on 3/30/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "Quadruped.h"

#include "math.h"
#include "drawingUtil.h"

void Quadruped::update(long dt) {
	float dt_f = dt/1000.0f; // number of seconds since last frame (decimal)
	
	
	// update walking animation
	float fSpeed = getFSpeed();
	if (fabsf(fSpeed) < 0.5) {
		// if idle, bring hands and feet to center
		long phase = util::mod(anim, M_PI*200);
		if (phase > M_PI*100) phase -= M_PI*200;
		anim -= phase * dt_f;
	} else {
		anim += dt * (fSpeed/2/*moveSpeed*/);
	}
	
	if (climbing)
		ySpeed = 0;
	else
		ySpeed -= dt_f*gravity; // apply gravity
	
	x += dt_f * xSpeed;
	y += dt_f * (climbing?moveSpeed:ySpeed);
	z += dt_f * zSpeed;
	
	//// perform collision detection/response
	/*/ walk up 1-block ledges without jumping
	{
		float t_x=x,t_y=y,t_z=z,t_lastX=lastX,t_lastY=lastY,t_lastZ=lastZ;
		lastCResult = doCollide();
		x=t_x;y=t_y;z=t_z;lastX=t_lastX;lastY=t_lastY;lastZ=t_lastZ;
	}
	if ((lastCResult.x||lastCResult.z)&&lastCResult.y) {
		y += 1.1;
		if (isColliding())
			y -= 1.1;
	}//*/
	lastCResult = doCollide();
	bool sliding = false; // used for applying friction
	if (lastCResult.y) {
		float absY = fabsf(ySpeed);
		if (absY > 20) {
			// fall damage
			damage((absY-20)*1.0);
		}
		ySpeed = 0;
		sliding = true;
	}
	if (lastCResult.x) {
		//xSpeed = 0;
		sliding = true;
	}
	if (lastCResult.z) {
		//zSpeed = 0;
		sliding = true;
	}
	climbing=(lastCResult.x||lastCResult.z) && moving;
	moving = false;
	
	// apply friction
	if (sliding && !climbing) {
		float fr = (dt_f*friction) + 1.0f;
		xSpeed /= fr;
		//ySpeed /= fr;
		zSpeed /= fr;
	}
}

void Quadruped::draw() {
	glPushMatrix();
	{
		glTranslatef(x, y, z);
		glRotatef(-yaw, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glColor4f(1, 1, 1, 1); // reset color (just in case)
		
		//// BODY ////
		drawBody();
		
		//// HEAD ////
		drawHead();
		
		float anim = sinf(Quadruped::anim/200.0f)/8.0f;
		
		//// ARMS ////
		glPushMatrix();
		{
			glTranslatef(0, 0, anim);
			drawArm(true);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0, 0, -anim);
			glScalef(-1, 1, 1);
			glDisable(GL_CULL_FACE);
			drawArm(false);
			glEnable(GL_CULL_FACE);
		}
		glPopMatrix();
		
		//// LEGS ////
		glPushMatrix();
		{
			glTranslatef(0, 0, -anim);
			drawLeg(true);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0, 0, anim);
			glScalef(-1, 1, 1);
			glDisable(GL_CULL_FACE);
			drawLeg(false);
			glEnable(GL_CULL_FACE);
		}
		glPopMatrix();
	}
	glPopMatrix();

}

void Quadruped::drawBody() {
	drawingUtil::drawBox(texCoords+(0*6*4*2), bodyDim.x,bodyDim.y,bodyDim.z, bodyOffset.x,bodyOffset.y,bodyOffset.z);
}

void Quadruped::drawHead() {
	drawingUtil::drawBox(texCoords+(1*6*4*2), headDim.x,headDim.y,headDim.z, headOffset.x,headOffset.y,headOffset.z);
}

void Quadruped::drawArm(bool left) {
	drawingUtil::drawBox(texCoords+(2*6*4*2), armDim.x,armDim.y,armDim.z, armOffset.x,armOffset.y,armOffset.z);
}

void Quadruped::drawLeg(bool left) {
	drawingUtil::drawBox(texCoords+(3*6*4*2), legDim.x,legDim.y,legDim.z, legOffset.x,legOffset.y,legOffset.z);
}
