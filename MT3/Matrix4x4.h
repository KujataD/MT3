#pragma once
#include <assert.h>
#include <cmath>
#include "Vector3.h"

class Matrix4x4 {
public:
	float m[4][4];

    Matrix4x4 operator+(const Matrix4x4& m) const;
	Matrix4x4 operator-(const Matrix4x4& m) const;
	Matrix4x4 operator*(const Matrix4x4& m) const;
	Matrix4x4 operator/(float scalar) const;

	static Matrix4x4 Inverse(const Matrix4x4& m);
	static Matrix4x4 Transpose(const Matrix4x4& m);
	static Matrix4x4 MakeIdentity();

	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	static Matrix4x4 MakeRotateXMatrix(float radian);
	static Matrix4x4 MakeRotateYMatrix(float radian);
	static Matrix4x4 MakeRotateZMatrix(float radian);
	
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
};
