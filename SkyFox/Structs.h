#pragma once
#include "Vector3d.h"
#include "SolidQuad.h"

struct Camera_t
{
	float fovy;
	float zNear;
	float zFar;
	Vector3d eye;
	Vector3d center;
	Vector3d up;
	Camera_t(float _fovy, float _zNear, float _zFar)
		:fovy(_fovy), zNear(_zNear), zFar(_zFar),
		eye(0, 0, 1),
		center(0, 0, eye.z+0.1f),
		up(0, 1, 0)
	{}
};

struct Background_t
{
	float circleDeltaCounter;
	float countRed;
	float countGreen;
	float countBlue;
	float countRed2;
	float countGreen2;
	float countBlue2;
	float colorRed;
	float colorGreen;
	float colorBlue;
	float colorRed2;
	float colorGreen2;
	float colorBlue2;
	float radius;
	float shininess;
	SolidQuad quad;
	float circleDelta;
	Background_t(float _circleDeltaCounter,
		float _countRed, float _countGreen, float _countBlue,
		float _countRed2, float _countGreen2, float _countBlue2,
		float _radius, float _shininess,
		float size_x, float size_y, float size_z,
		float center_x, float center_y, float center_z,
		bool center_is_hind)
		:circleDeltaCounter(_circleDeltaCounter),
		countRed(_countRed),
		countGreen(_countGreen),
		countBlue(_countBlue),
		countRed2(_countRed2),
		countGreen2(_countGreen2),
		countBlue2(_countBlue2),
		radius(_radius),
		shininess(_shininess),
		quad(size_x, size_y, size_z,
			center_x, center_y, center_z),
		circleDelta(0.0f)
	{}
};