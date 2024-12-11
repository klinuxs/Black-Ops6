#pragma once

#include "Helper.h"

struct Vector2
{
	float x, y;

	Vector2 operator-(Vector2 ape)
	{
		return { x - ape.x, y - ape.y};
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}
	float Dist2D(Vector2 ape)
	{
		return (*this - ape).Length2D();
	}
};

struct Vector3
{
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {}

	// Constructor for initialization
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	Vector3 operator-(Vector3 ape)
	{
		return { x - ape.x, y - ape.y, z - ape.z };
	}

	Vector3 operator+(Vector3 ape)
	{
		return { x + ape.x, y + ape.y, z + ape.z };
	}

	Vector3 operator*(float ape)
	{
		return { x * ape, y * ape, z * ape };
	}

	Vector3 operator/(float ape)
	{
		return { x / ape, y / ape, z / ape };
	}

	Vector3 operator/=(float ape)
	{
		x /= ape;
		y /= ape;
		z /= ape;

		return *this;
	}

	Vector3 operator+=(Vector3 ape)
	{
		return { x += ape.x, y += ape.y, z += ape.z };
	}

	Vector3 operator-=(Vector3 ape)
	{
		return { x -= ape.x, y -= ape.y, z -= ape.z };
	}

	float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	float Length2D()
	{
		return sqrt((x * x) + (y * y));
	}

	float DistTo(Vector3 ape)
	{
		return (*this - ape).Length();
	}

	float Dist2D(Vector3 ape)
	{
		return (*this - ape).Length2D();
	}

	float Dot(Vector3& v)
	{
		return x * v.x + y * v.y + z * v.z;
	}
};

struct key {
	int32_t ref0;
	int32_t ref1;
	int32_t ref2;
};

struct ref_def_t {
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
	Vector2 tan_half_fov;
	char pad1[8];  // 8-byte padding
	char pad2[4];  // 4-byte padding
	Vector3 axis[3];
};

struct FMinimalViewInfo {
	Vector3 Location;
	Vector3 Rotation;
	float FOV;
};

struct FCameraCacheEntry {
	char pad_0x0[0x10];
	FMinimalViewInfo POV;
};