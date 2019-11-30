#pragma once
class Vector3d
{
public:
	float x;
	float y;
	float z;
	// Constructors
	Vector3d(){};
	Vector3d(float _x, float _y, float _z)
		:x(_x), y(_y), z(_z)
	{};
	// Destructor
	~Vector3d() {};

	// Overloaded operators
	void operator += (float v) {
		x += v;
		y += v;
		z += v;
	}

	void operator += (Vector3d *vb) {
		x += vb->x;
		y += vb->y;
		z += vb->z;
	}

	void operator -= (float v) {
		x -= v;
		y -= v;
		z -= v;
	}

	void operator -= (Vector3d* vb) {
		x -= vb->x;
		y -= vb->y;
		z -= vb->z;
	}

	void operator *= (float v) {
		x *= v;
		y *= v;
		z *= v;
	}

	void operator *= (Vector3d* vb) {
		x *= vb->x;
		y *= vb->y;
		z *= vb->z;
	}

	void operator /= (float v) {
		x /= v;
		y /= v;
		z /= v;
	}

	void operator /= (Vector3d* vb) {
		x /= vb->x;
		y /= vb->y;
		z /= vb->z;
	}
};

