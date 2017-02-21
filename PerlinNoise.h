//
//  PerlinNoise.h
//  Bedrock
//
//  Created by Quinn on 10/12/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include "NoiseGrid.h"

#include <vector>

class PerlinNoise {
public:
	PerlinNoise(int w, int h, int oct, float per, unsigned seed, NoiseGrid::Interpolation iType);
	
	float getNoise(float x, float y);
	
	const int width, height, octaves;
	const float persistence; // persistence of < 1 means less high frequency
	
protected:
	std::vector<NoiseGrid> noise;
};