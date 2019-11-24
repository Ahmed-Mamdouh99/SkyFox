#pragma once
#include "Vector3d.h"
class SolidQuad
{
public:
	Vector3d size;
	Vector3d center;
	Vector3d center_hind;
	SolidQuad(float size_x, float size_y, float size_z,
		float center_x, float center_y, float center_z, bool center_is_hind = false);
	// Translate the solid's centers by a vector
	void move(Vector3d* delta);
	// Translate the solid's centers by a value on all dimensions
	void move(float delta);
	// Test collision with another quad
	bool intersect(SolidQuad* quad2);
};