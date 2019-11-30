#pragma once
#include "Vector3d.h"
class SolidQuad
{
public:
	Vector3d size;
	Vector3d center;
	Vector3d center_hind;
	Vector3d rotation;
	SolidQuad(float size_x=1.0f, float size_y=1.0f, float size_z=1.0f,
		float center_x=0.0f, float center_y=0.0f, float center_z=0.0f, bool center_is_hind = false);
	// Test collision with another quad
	bool intersect(SolidQuad* quad2);
};