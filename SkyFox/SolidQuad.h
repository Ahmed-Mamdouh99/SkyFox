#pragma once
#include "Vector3d.h"
#include <math.h>
#include <algorithm>
class SolidQuad
{
public:
	SolidQuad(float sizeX = 1.0f, float sizeY = 1.0f, float sizeZ = 1.0f,
		float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f,
		float rotationX = 0.0f, float rotationY = 0.0f, float rotationZ = 0.0f);
	// Test collision with another quad
	bool intersect(SolidQuad* quad2);
	Vector3d size;
	Vector3d center;
	Vector3d rotation;
};