#pragma once
#include <time.h>
#include <random>

namespace rng {

	static bool initialized = false;

	static float roll(float lower_bound=0.0f, float upper_bound=1.0f)
	{
		if (!initialized) {
			srand(time(NULL));
			initialized = true;
		}
		float range = upper_bound - lower_bound;
		return range * ((float)rand()) / RAND_MAX + lower_bound;
	}
};