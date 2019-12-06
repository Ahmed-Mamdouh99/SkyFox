#include "SolidQuad.h"

SolidQuad::SolidQuad(float sizeX, float sizeY, float sizeZ,
	float centerX, float centerY, float centerZ,
	float rotationX, float rotationY, float rotationZ) :
	size(sizeX, sizeY, sizeZ),
	center(centerX, centerY, centerZ),
	rotation(rotationX, rotationY, rotationZ)
{};

// Test collision with another quad
bool SolidQuad::intersect(SolidQuad* quad2)
{
	// Using AABB (Axis Aligned Bounding Box) check
	float rad = std::min(pow(size.x, 2)+ pow(size.y, 2) + pow(size.y, 2),
		pow(quad2->size.x, 2) + pow(quad2->size.y, 2) + pow(quad2->size.y, 2));
	float delta = pow(center.x - quad2->center.x, 2) + pow(center.y - quad2->center.y, 2)
		+ pow(center.z - quad2->center.z, 2);
	return delta <= rad;
}