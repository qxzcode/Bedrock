//
//  Unit.h
//  Bedrock
//
//  Created by Quinn on 11/3/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "Quadruped.h"
class Unit;
#include "globals.h"
#include "Command.h"
#include "Item.h"

#include <queue>
#include <map>

#define UNIT_SIZE /**1.0f, 1.0f, 1.0f//*/0.6875f, 0.6875f, 0.6875f

class Unit: public Quadruped {
public:
	
	///////////
	// BASIC //
	///////////
	
	Unit(float x, float y, float z, World * w);
	std::string name(){return"Unit"/*unit's name...*/+healthStr();}
	
	void update(long dt);
	void draw();
protected:
	void drawArm(bool left); // overridden to draw held items
public:
	
	float viewDistance = 
			#ifdef WORLD_VIEW
				WORLD_X;
			#else
				55.0f;
			#endif
	
	friend class Player;
	
	
	///////////
	// ITEMS //
	///////////
	
	Item* rightItem = NULL;
	Item* leftItem = NULL;
	
	
	/////////////////
	// OCCUPATIONS //
	/////////////////
	
	enum struct Occupation {
		librarian, miner
	};
	static int getOccSkillPoints(Occupation o) {
		switch (o) {
			case Occupation::librarian:	return 100;
			case Occupation::miner:		return 80;
		}
	}
	static std::string getOccName(Occupation o) {
		switch (o) {
			case Occupation::librarian:	return "Librarian";
			case Occupation::miner:		return "Miner";
		}
	}
	
	int skillPoints = 100;
	int getRemainingSP() {
		int sp = skillPoints;
		for (std::pair<Occupation,int> pair : occupations)
			sp -= getOccSkillPoints(pair.first);
		return sp;
	}
	
	std::map<Occupation, int> occupations; // key is occupation, value is level
	
	
	//////////////
	// COMMANDS //
	//////////////
	
	std::queue<Command*> cmdQueue;
	friend class Command;
};
