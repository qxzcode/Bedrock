//
//  hud.cpp
//  Bedrock
//
//  Created by Quinn on 1/16/14.
//  Copyright (c) 2014 Hexahedron Games. All rights reserved.
//

#include "hud.h"

#include "ortho.h"
#include "Block.h"
#include "globals.h"
#include "input.h"
#include "Button.h"
#include "MenuButton.h"
#include "Menu.h"


//// VARIABLES ////
#define HUD_WIDTH 250
#define HUD_HEIGHT 400
#define BG_GRAY 0.2
const ortho::color bgColor = {BG_GRAY, BG_GRAY, BG_GRAY, 0.75};
#define BUTTON_GRAY 0.4
const ortho::color buttonColor = {BUTTON_GRAY, BUTTON_GRAY, BUTTON_GRAY, 0.75};
#define BUTTON_HOVER_GRAY 0.5
const ortho::color buttonHoverColor = {BUTTON_HOVER_GRAY, BUTTON_HOVER_GRAY, BUTTON_HOVER_GRAY, 0.75};

DisplayList hud::hudBG;

// ELEMENT CREATION //
#define BUTTON_ITEM(text,onClick,onClickArg,greenText) \
				new Button(text, onClick,onClickArg, 0,0,0,30, buttonColor,buttonHoverColor,greenText?ortho::green:ortho::white)
#define MENU_ITEM(text,menu) \
				new MenuButton(text, 0,0,0,30, buttonColor,buttonHoverColor,ortho::white, menu)
//////////////////////

static Menu unitsMenu(10+HUD_WIDTH, 10, HUD_WIDTH, HUD_HEIGHT, bgColor);
static MenuButton unitsButton("UNITS", 10, 10+140, HUD_WIDTH, 30, buttonColor, buttonHoverColor, ortho::white, unitsMenu);

static Menu actionsMenu(10+HUD_WIDTH, 10, HUD_WIDTH, HUD_HEIGHT, bgColor);
static MenuButton actionsButton("ACTIONS", 10, 10+250, HUD_WIDTH, 30, buttonColor, buttonHoverColor, ortho::white, actionsMenu);

static void cancel(int i) { hud::issuingCmd = hud::CommandEnum::NONE; }
static Button cancelButton("CANCEL", cancel,0, 0,50,100,30, bgColor,buttonColor,ortho::white);

// OCCUPATION MENUS //
static Menu librarianMenu(10+HUD_WIDTH+actionsMenu.width-10, 10, HUD_WIDTH, HUD_HEIGHT, bgColor);
static Menu minerMenu(10+HUD_WIDTH+actionsMenu.width-10, 10, HUD_WIDTH, HUD_HEIGHT, bgColor);

using hud::CommandEnum;
CommandEnum hud::issuingCmd = CommandEnum::NONE;
static void issueCommand(int cmd) {
	switch (cmd) {
		case (int)CommandEnum::CANCEL:
			if (curUnit->cmdQueue.size() > 0) {
				delete curUnit->cmdQueue.front(); // delete Command object
				curUnit->cmdQueue.pop(); // remove the Command from the queue
			}
			break;
			
		default:
			hud::issuingCmd = (hud::CommandEnum)cmd;
			break;
	}
	// close menus
	librarianMenu.isOpen = false;
	minerMenu.isOpen = false;
	actionsMenu.isOpen = false;
}
static void otherUnitAction(int action) {
	switch (action) {
		// toggle controllingUnit state
		case 1:
			player.controllingUnit = !player.controllingUnit;
			hud::buildMenus();
			break;
	}
	// close menus
	librarianMenu.isOpen = false;
	minerMenu.isOpen = false;
	actionsMenu.isOpen = false;
}

static bool areOccMenusInitialized = false;
static void initOccupationMenus() {
	using hud::CommandEnum;
	
	// librarian menu
	//...
	
	// miner menu
	minerMenu.addItem(BUTTON_ITEM("MINE", issueCommand, (int)CommandEnum::MINE, false));
	
	areOccMenusInitialized = true;
}
//////////////////////


static void selectUnit(int unit) {
	unitsMenu.isOpen = false;
	curUnit = units[unit];
	hud::buildMenus();
}


void hud::buildMenus() {
	// UNITS menu
	unitsMenu.clearItems();
	for (int i = 0; i < units.size(); i++) {
		unitsMenu.addItem(BUTTON_ITEM("UNIT "+ortho::toString(i+1), selectUnit, i, units[i]==curUnit));
	}
	
	// ACTIONS menu
	actionsMenu.clearItems();
	actionsMenu.addItem(BUTTON_ITEM(player.controllingUnit?"EXIT 1ST PERSON":"ENTER 1ST PERSON", otherUnitAction, 1, true));
	actionsMenu.addItem(BUTTON_ITEM("CANCEL", issueCommand, (int)CommandEnum::CANCEL, false));
	actionsMenu.addItem(BUTTON_ITEM("MOVE", issueCommand, (int)CommandEnum::MOVE, false));
	if (!areOccMenusInitialized) initOccupationMenus();
	for (std::pair<Unit::Occupation, int> pair : curUnit->occupations) {
		Unit::Occupation occ = pair.first;
		switch (occ) {
			case Unit::Occupation::librarian:
				actionsMenu.addItem(MENU_ITEM(Unit::getOccName(occ), librarianMenu));
				break;
			case Unit::Occupation::miner:
				actionsMenu.addItem(MENU_ITEM(Unit::getOccName(occ), minerMenu));
				break;
		}
	}
}








bool hud::draw(std::string selection, int fps) {
	bool inHUD = false;
	
	ortho::orthoOn();
	{
		if ((int)issuingCmd) {
			// display help text at top of window
			std::string helpText;
			switch (issuingCmd) {
				case CommandEnum::MOVE:
					helpText = "Click on the block to move to";
					break;
				case CommandEnum::MINE:
					helpText = "Click on the two corners";
					break;
					
				default: break;
			}
			ortho::drawText(helpText, CENTER_TEXT_WIN(helpText, 2), 25, 2);
			cancelButton.xPos = (width/2) - (cancelButton.width/2);
			inHUD = cancelButton.draw(input::mouseX, input::mouseY);
			
		} else {
			
			inHUD = input::mouseX>=10 && input::mouseX<=10+HUD_WIDTH &&
					input::mouseY>=10 && input::mouseY<=10+HUD_HEIGHT;
			
			ortho::drawRect(bgColor, 10, 10, HUD_WIDTH, HUD_HEIGHT);
			
			// unit's name
			const std::string unitName = "UNIT NAME";
			ortho::drawString(unitName, CENTER_TEXT(unitName, 2, 10+(HUD_WIDTH/2)), 10+10, 2, ortho::white);
			
			// unit's picture
			//...
			
			// "UNITS" button
			inHUD |= unitsButton.draw(input::mouseX, input::mouseY);
			
			// stats
#define BAR_BORDER 2
#define BAR_COLOR(frac) frac>0.5? ortho::green: frac>0.25? ortho::orange: ortho::red
			// health
			std::string healthStr = std::string("HEALTH (") + curUnit->getDisplayHealth() + "/" + (int)curUnit->maxHealth + ")";
			float healthFrac = curUnit->health/curUnit->maxHealth;
			ortho::drawString(healthStr, 10+10, 10+140+40, 2, ortho::white);
			ortho::drawRect(bgColor, 10+10-BAR_BORDER, 10+140+40+24-BAR_BORDER, HUD_WIDTH-20+(BAR_BORDER*2), 10+(BAR_BORDER*2));
			ortho::drawRect(BAR_COLOR(healthFrac), 10+10, 10+140+40+24, (HUD_WIDTH-20)*healthFrac, 10);
			
			// "ACTIONS" button
			inHUD |= actionsButton.draw(input::mouseX, input::mouseY);
		}
		
		
		
		////// EXTRAS //////
		
		// display selected block or item's name
		if (!selection.empty() && !inHUD) {
			ortho::drawText(selection, CENTER_TEXT_WIN(selection, 2), height-75, 2);
		}
		
		// display FPS, terrain quad count, and particle count
		ortho::drawText("QUADS: "+ortho::toString(terrainQuadsRendered), width-300, 10, 2);
		ortho::drawText("FPS: "+ortho::toString(fps), width-300, 30, 2);
		ortho::drawText("PARTICLES: "+ortho::toString(world.particleCount)+"/"+ortho::toString(MAX_PARTICLES), width-300, 50, 2);
		//ortho::drawText("PLAYER Y: "+ortho::toString((int)curUnit->y), width-200, 50, 2);
	}
	ortho::orthoOff();
	
	return inHUD;
}


