//
//  PerlinNoise.cpp
//  Bedrock
//
//  Created by Quinn on 10/12/13.
//  Copyright (c) 2013 Hexahedron Games. All rights reserved.
//

#include "PerlinNoise.h"

#include <stdlib.h>	// random, srandom

PerlinNoise::PerlinNoise(int w, int h, int oct, float per, unsigned seed, NoiseGrid::Interpolation iType):width(w),height(h),octaves(oct),persistence(per) {
	srandom(seed);
	float freq = 1;
	for (int o = 0; o < octaves; o++,freq*=2) {
		noise.push_back(NoiseGrid(width * freq, height * freq, (unsigned)random(), iType));
	}
}

float PerlinNoise::getNoise(float x, float y) {
	float sum = 0;
	float freq = 1;
	float amp = 1;
	for (int o = 0; o < octaves; o++,freq*=2,amp*=persistence) {
		sum += noise[o].getNoise(x * freq, y * freq) * amp;
	}
	return sum;
}