#pragma once
template <class T>
class Vector3d
{
public:
	T x;
	T y;
	T z;
	// Constructors
	Vector3d(){};
	Vector3d(T _x, T _y, T _z)
		:x(_x), y(_y), z(_z)
	{};
	// Destructor
	~Vector3d() {};

	// Overloaded operators
	void operator += (T v) {
		x += v;
		y += v;
		z += v;
	}

	void operator += (Vector3d<T> *vb) {
		x += vb->x;
		y += vb->y;
		z += vb->z;
	}

	void operator -= (T v) {
		x -= v;
		y -= v;
		z -= v;
	}

	void operator -= (Vector3d<T>* vb) {
		x -= vb->x;
		y -= vb->y;
		z -= vb->z;
	}

	void operator *= (T v) {
		x *= v;
		y *= v;
		z *= v;
	}

	void operator *= (Vector3d<T>* vb) {
		x *= vb->x;
		y *= vb->y;
		z *= vb->z;
	}

	void operator /= (T v) {
		x /= v;
		y /= v;
		z /= v;
	}

	void operator /= (Vector3d<T>* vb) {
		x /= vb->x;
		y /= vb->y;
		z /= vb->z;
	}
};

