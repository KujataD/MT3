#include "Quaternion.h"
#include <cmath>

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
	return {
	    lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
	    lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
	    lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
	    lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
	};
}

Quaternion IdentityQuaternion() { return {0.0f, 0.0f, 0.0f, 1.0f}; }

Quaternion Conjugate(const Quaternion& quaternion) { return {-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w}; }

float Norm(const Quaternion& quaternion) {
	return std::sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}

Quaternion Normalize(const Quaternion& quaternion) {
	const float norm = Norm(quaternion);
	return {quaternion.x / norm, quaternion.y / norm, quaternion.z / norm, quaternion.w / norm};
}

Quaternion Inverse(const Quaternion& quaternion) {
	const Quaternion conjugate = Conjugate(quaternion);
	const float norm = Norm(quaternion);
	const float normSquared = norm * norm;
	return {conjugate.x / normSquared, conjugate.y / normSquared, conjugate.z / normSquared, conjugate.w / normSquared};
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	const float halfAngle = angle / 2.0f;
	const float sinHalf = std::sin(halfAngle);
	return {axis.x * sinHalf, axis.y * sinHalf, axis.z * sinHalf, std::cos(halfAngle)};
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	const Quaternion rotated = Multiply(Multiply(quaternion, {vector.x, vector.y, vector.z, 0.0f}), Conjugate(quaternion));
	return {rotated.x, rotated.y, rotated.z};
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion) {
	const float x = quaternion.x;
	const float y = quaternion.y;
	const float z = quaternion.z;
	const float w = quaternion.w;
	return {
	    {{w * w + x * x - y * y - z * z, 2.0f * (x * y + w * z), 2.0f * (x * z - w * y), 0.0f},
	     {2.0f * (x * y - w * z), w * w - x * x + y * y - z * z, 2.0f * (y * z + w * x), 0.0f},
	     {2.0f * (x * z + w * y), 2.0f * (y * z - w * x), w * w - x * x - y * y + z * z, 0.0f},
	     {0.0f, 0.0f, 0.0f, 1.0f}}
    };
}
