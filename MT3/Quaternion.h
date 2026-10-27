#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

// Quaternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
// 乗法単位元を返す
Quaternion IdentityQuaternion();
// 共役Quaternionを返す
Quaternion Conjugate(const Quaternion& quaternion);
// Quaternionのnormを返す
float Norm(const Quaternion& quaternion);
// 正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& quaternion);
// 逆Quaternionを返す
Quaternion Inverse(const Quaternion& quaternion);

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);
