#pragma once
#include "SolidQuad.h"

class MovingQuad : public SolidQuad
{
public:
	MovingQuad(float sizeX = 1.0f, float sizeY = 1.0f, float sizeZ = 1.0f,
		float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f,
		float dx=0.0f, float  dy = 0.0f, float dz = 0.0f, float rx = 0.0f, float ry = 0.0f, float rz = 0.0f);
	Vector3d movementVector;
	Vector3d rotationVector;
};