#include "SolidQuad.h"


SolidQuad::SolidQuad(float size_x, float size_y, float size_z,
	float center_x, float center_y, float center_z, bool center_is_hind) :
	size(size_x, size_y, size_z)
{
	// Set solid centers
	if (center_is_hind) {
		center_hind.x = center_x;
		center_hind.y = center_y;
		center_hind.z = center_z;
		center.x = center_x + size.x / 2;
		center.y = center_y + size.y / 2;
		center.z = center_z + size.z / 2;
	}
	else {
		center.x = center_x;
		center.y = center_y;
		center.z = center_z;
		center_hind.x = center_x - size.x / 2;
		center_hind.y = center_y - size.y / 2;
		center_hind.z = center_z - size.z / 2;
	}
};

void SolidQuad::move(Vector3d<float>* delta)
{
	center += delta;
	center_hind += delta;
}
// Translate the solid's centers by a value on all dimensions
void SolidQuad::move(float delta)
{
	center += delta;
	center_hind += delta;
}

// Test collision with another quad
bool SolidQuad::intersect(SolidQuad* quad2)
{
	float max_x = center_hind.x + size.x;
	float max_y = center_hind.y + size.y;
	float max_z = center_hind.z + size.z;
	float min_x = center_hind.x;
	float min_y = center_hind.y;
	float min_z = center_hind.z;
	return (
		quad2->center.x >= min_x && quad2->center.x <= max_x &&
		quad2->center.y >= min_y && quad2->center.y <= max_y &&
		quad2->center.z >= min_z && quad2->center.z <= max_z
	);
}