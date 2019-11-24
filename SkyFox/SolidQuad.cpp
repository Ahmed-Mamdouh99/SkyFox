#include "SolidQuad.h"
#include <math.h>

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

void SolidQuad::move(Vector3d* delta)
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
	// Using AABB (Axis Aligned Bounding Box) check
	return (
		fabsf(center.x - quad2->center.x) < (size.x + quad2->size.x) &&
		fabsf(center.y - quad2->center.y) < (size.y + quad2->size.y) &&
		fabsf(center.z - quad2->center.z) < (size.z + quad2->size.z)
	);
}