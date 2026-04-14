#include "Vector3.h"
#include "Matrix4x4.h"

Vector3 Vector3::Transform(const Vector3& v, const Matrix4x4& m) { 
	Vector3 result;
	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + 1.0f * m.m[3][0];
	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + 1.0f * m.m[3][1];
	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + 1.0f * m.m[3][2];
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + 1.0f * m.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Vector3 Vector3::Project(const Vector3& a, const Vector3& b) { 
	float dotAB = Dot(a, b); 
	float fieldNormB = std::powf(Length(b), 2);
	float dotNormAB = dotAB / fieldNormB;
	Vector3 result = b * dotNormAB;
	return result;
}

Vector3 Vector3::ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 a = point - segment.origin;
	float t = Dot(a, segment.diff) / std::powf(Length(segment.diff), 2); 
	t = std::clamp(t, 0.0f, 1.0f);
	Vector3 cp = segment.origin + segment.diff * t;
	return cp;
}

Vector3 Vector3::Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return {-vector.y, vector.x, 0.0f};
	}
	return {0.0f, -vector.z, vector.y};
}
