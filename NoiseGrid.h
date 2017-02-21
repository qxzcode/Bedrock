//
//  NoiseGrid.h
//  Bedrock
//
//  Created by Quinn on 10/12/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#pragma once

#include <vector>

class NoiseGrid {
public:
	enum Interpolation { LINEAR, COSINE };
	
	NoiseGrid(int w, int h, unsigned seed, Interpolation i);
	
	float getNoise(float x, float y);
	float interpolate(float a, float b, float x);
	
	const int width, height;
	const Interpolation iType;
	
protected:
	std::vector<std::vector<float>> grid;
};