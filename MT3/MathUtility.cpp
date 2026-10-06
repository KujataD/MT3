#include "MathUtility.h"

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return {
	    v1.x + (v2.x - v1.x) * t,
	    v1.y + (v2.y - v1.y) * t,
	    v1.z + (v2.z - v1.z) * t,
	};
}

Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t) {
	const Vector3 a = Lerp(p0, p1, t);
	const Vector3 b = Lerp(p1, p2, t);
	return Lerp(a, b, t);
}

Vector3 Reflect(const Vector3& input, const Vector3& normal) {
	return input - normal * (2.0f * Dot(input, normal));
}
