#pragma once
#include "Vector3d.h"

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
	Background_t(float _countRed, float _countGreen,
		float _countBlue, float _radius, float _shininess)
		:countRed(_countRed),
		countGreen(_countGreen),
		countBlue(_countBlue),
		radius(_radius),
		shininess(_shininess){}
};