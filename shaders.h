//
//  shaders.h
//  Bedrock
//
//  Created by Quinn on 11/2/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "ShaderProgram.h"

namespace shaders {
	void load(); // loads shaders
	
	extern ShaderProgram fog, plain, ortho;
	extern GLint fog_terrainLoc, fog_texturesLoc, fog_fogOnLoc, fog_selectLoc, fog_flowerCenterLoc, fog_flowerPetalsLoc, fog_fogCenterLoc, fog_fogEndLoc, fog_viewMatrixLoc, fog_grassWaveLoc, fog_plainLoc, fog_debugLoc, fog_facePositionLoc, fog_particleColorSizeLoc,
				 plain_viewMatrixLoc;
	
	extern ShaderProgram * curProg;
}