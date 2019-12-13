#include "MovingQuad.h"

MovingQuad::MovingQuad(
	float sizeX, float sizeY, float sizeZ,
	float centerX, float centerY, float centerZ,
	float dx, float  dy, float dz,
	float rx, float ry, float rz)
: SolidQuad(sizeX, sizeY, sizeZ, centerX, centerY, centerZ),
movementVector(dx, dy, dz),
rotationVector(rx, ry, rz)
{}