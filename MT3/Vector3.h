#pragma once
#include <assert.h>
#include <cmath>

class Matrix4x4;

class Vector3 {
public:
	float x;
	float y;
	float z;

	Vector3 operator+(const Vector3& v) const { return {x + v.x, y + v.y, z + v.z}; }
	Vector3 operator-(const Vector3& v) const { return {x - v.x, y - v.y, z - v.z}; }
	Vector3 operator*(float scalar) const { return {x * scalar, y * scalar, z * scalar}; }
	Vector3 operator/(float scalar) const { return {x / scalar, y / scalar, z / scalar}; }

	void operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	void operator-=(const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	void operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}
	void operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
	}

	static float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
	static float Length(const Vector3& v) { return std::sqrt(Dot(v, v)); }
	static Vector3 Normalize(const Vector3& v) { return v / Length(v); }
	static Vector3 Transform(const Vector3& v, const Matrix4x4& m);
};
