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
	float countRed;
	float countGreen;
	float countBlue;
	float colorRed;
	float colorGreen;
	float colorBlue;
	float radius;
	float shininess;
	SolidQuad quad;
	Background_t(float _countRed, float _countGreen,
		float _countBlue, float _radius, float _shininess,
		float size_x, float size_y, float size_z,
		float center_x, float center_y, float center_z,
		bool center_is_hind)
		:countRed(_countRed),
		countGreen(_countGreen),
		countBlue(_countBlue),
		radius(_radius),
		shininess(_shininess),
		quad(size_x, size_y, size_z,
			center_x, center_y, center_z,
			center_is_hind)
	{}
};