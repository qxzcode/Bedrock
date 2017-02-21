//
//  input.cpp
//  Bedrock
//
//  Created by Quinn on 9/30/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "input.h"

void input::handleEvent(SDL_Event& event) {
	SDLKey key;
	switch (event.type) {
			
		/// MOUSE BUTTON
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_RIGHT)
				mouseRightDown = true;
			if (event.button.button == SDL_BUTTON_LEFT)
				mouseLeftClicked = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_RIGHT)
				mouseRightDown = false;
			break;
			
		/// MOUSE MOTION
		case SDL_MOUSEMOTION:
			if (mouseRightDown) { // only increment these when dragging the right mouse button
				mouseDragDX += event.motion.xrel;
				mouseDragDY += event.motion.yrel;
			}
			mouseX = event.motion.x;
			mouseY = event.motion.y;
			break;
			
		/// KEYBOARD
		case SDL_KEYDOWN:
			key = event.key.keysym.sym;
			if (key == SDLK_w)		playerForwards = true;
			if (key == SDLK_s)		playerBackwards = true;
			if (key == SDLK_a)		playerLeft = true;
			if (key == SDLK_d)		playerRight = true;
			if (key == SDLK_LSHIFT)	playerDown = true;
			if (key == SDLK_SPACE)	playerUp = true;
			if (key == SDLK_q)		dropItem = true;
			if (key == SDLK_e)		switchArms = true;
			if (key == SDLK_ESCAPE)	quit = true;
			if (key == SDLK_CAPSLOCK)debug = true;
			if (key == SDLK_f)		fog = !fog;
			break;
		case SDL_KEYUP:
			key = event.key.keysym.sym;
			if (key == SDLK_w)		playerForwards = false;
			if (key == SDLK_s)		playerBackwards = false;
			if (key == SDLK_a)		playerLeft = false;
			if (key == SDLK_d)		playerRight = false;
			if (key == SDLK_LSHIFT)	playerDown = false;
			if (key == SDLK_SPACE)	playerUp = false;
			//if (key == SDLK_ESCAPE)	quit = false;
			if (key == SDLK_CAPSLOCK)debug = false;
			break;
			
	}
}

bool input::playerForwards = false,
	 input::playerBackwards = false,
	 input::playerLeft = false,
	 input::playerRight = false,
	 input::playerDown = false,
	 input::playerUp = false,
	 input::dropItem = false,
	 input::switchArms = false,
	 input::quit = false,
	 input::debug = false,
	 input::fog = true;

int input::mouseX = 0,
	input::mouseY = 0,
	input::mouseDragDX = 0,
	input::mouseDragDY = 0;

bool input::mouseRightDown = false,
	 input::mouseLeftClicked = false;
