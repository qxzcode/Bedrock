//
//  NoiseGrid.cpp
//  Bedrock
//
//  Created by Quinn on 10/12/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "NoiseGrid.h"

#include <stdlib.h>
#include "math.h"

NoiseGrid::NoiseGrid(int w, int h, unsigned seed, Interpolation i):width(w),height(h),iType(i),grid(width, std::vector<float>()) {
	srandom(seed);
	
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			grid[x].push_back(float(random()) / float(RAND_MAX));
	/*for (int x = 0; x < width; x++)
		grid[x][height-1] = grid[x][0];
	for (int y = 0; y < width; y++)
		grid[width-1][y] = grid[0][y];*/
}

float NoiseGrid::getNoise(float x, float y) {
	if (x < 0 || y < 0 || x >= width || y >= height) {
		printf("Tried to access out of bounds value of NoiseGrid: (%f, %f)\nReturning 0.\n", x, y);
		return 0;
	}
	int   ix = x,
		  iy = y;
	float fx = x - ix,
		  fy = y - iy;
	
	int xMin = ix, yMin = iy;
	int xMax, yMax;
	if (ix==width-1) {
		xMax = 0;
	} else {
		xMax = xMin + 1;
	}
	if (iy==height-1) {
		yMax = 0;
	} else {
		yMax = yMin + 1;
	}
	
	return interpolate(interpolate(grid[xMin][yMin],	grid[xMax][yMin],	fx),
					   interpolate(grid[xMin][yMax],	grid[xMax][yMax],	fx),
					   fy);
}

float NoiseGrid::interpolate(float a, float b, float x) {
	switch (iType) {
		case NoiseGrid::Interpolation::LINEAR:
			return a + ((b-a) * x);
		case NoiseGrid::Interpolation::COSINE:
			float x2 = (1-cos(x*M_PI))/2;
			return a*(1-x2) + b*x2;
	}
}