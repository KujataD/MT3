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
