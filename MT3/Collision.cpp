#include "Collision.h"

#include <algorithm>
#include <cmath>
#include <utility>

namespace {

bool IsOverlappingOnAxis(const OBB& obb1, const OBB& obb2, const Vector3& axis) {
	const Vector3 normalizedAxis = Vector3::Normalize(axis);

	auto getProjection = [&normalizedAxis](const OBB& obb) {
		const float center = Vector3::Dot(obb.center, normalizedAxis);
		const float extent =
		    std::abs(Vector3::Dot(obb.orientations[0], normalizedAxis)) * obb.size.x + std::abs(Vector3::Dot(obb.orientations[1], normalizedAxis)) * obb.size.y +
		    std::abs(Vector3::Dot(obb.orientations[2], normalizedAxis)) * obb.size.z;
		return std::make_pair(center - extent, center + extent);
	};

	const auto [min1, max1] = getProjection(obb1);
	const auto [min2, max2] = getProjection(obb2);
	const float sumSpan = (max1 - min1) + (max2 - min2);
	const float longSpan = (std::max)(max1, max2) - (std::min)(min1, min2);
	return sumSpan >= longSpan;
}

} // namespace

bool IsCollision(const Sphere& sphere, const Plane& plane) {
	const float distance = std::abs(Vector3::Dot(plane.normal, sphere.center) - plane.distance);
	return distance <= sphere.radius;
}

bool IsCollision(const Segment& segment, const Plane& plane) {
	const float dot = Vector3::Dot(plane.normal, segment.diff);
	if (dot == 0.0f) {
		return false;
	}

	const float t = (plane.distance - Vector3::Dot(segment.origin, plane.normal)) / dot;
	return t >= 0.0f && t <= 1.0f;
}

bool IsCollision(const Segment& segment, const Triangle& triangle) {
	const Vector3 normal = Vector3::Normalize(Vector3::Cross(triangle.vertices[1] - triangle.vertices[0], triangle.vertices[2] - triangle.vertices[0]));
	const float dot = Vector3::Dot(normal, segment.diff);
	if (dot == 0.0f) {
		return false;
	}

	const float d = Vector3::Dot(normal, triangle.vertices[0]);
	const float t = (d - Vector3::Dot(segment.origin, normal)) / dot;
	const Vector3 p = segment.origin + segment.diff * t;

	const Vector3 v01 = triangle.vertices[1] - triangle.vertices[0];
	const Vector3 v12 = triangle.vertices[2] - triangle.vertices[1];
	const Vector3 v20 = triangle.vertices[0] - triangle.vertices[2];
	const Vector3 cross01 = Vector3::Cross(v01, p - triangle.vertices[1]);
	const Vector3 cross12 = Vector3::Cross(v12, p - triangle.vertices[2]);
	const Vector3 cross20 = Vector3::Cross(v20, p - triangle.vertices[0]);

	const bool isInside = Vector3::Dot(cross01, normal) >= 0.0f && Vector3::Dot(cross12, normal) >= 0.0f && Vector3::Dot(cross20, normal) >= 0.0f;
	const bool isOnSegment = t >= 0.0f && t <= 1.0f;
	return isInside && isOnSegment;
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	const bool overlapsX = aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x;
	const bool overlapsY = aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y;
	const bool overlapsZ = aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z;
	return overlapsX && overlapsY && overlapsZ;
}

bool IsCollision(const AABB& aabb, const Sphere& sphere) {
	// AABB上で球の中心に最も近い点を求める
	const Vector3 closestPoint{
	    std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
	    std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
	    std::clamp(sphere.center.z, aabb.min.z, aabb.max.z),
	};
	return Vector3::Length(closestPoint - sphere.center) <= sphere.radius;
}

bool IsCollision(const AABB& aabb, const Segment& segment) {
	const float txMin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	const float txMax = (aabb.max.x - segment.origin.x) / segment.diff.x;
	const float tyMin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	const float tyMax = (aabb.max.y - segment.origin.y) / segment.diff.y;
	const float tzMin = (aabb.min.z - segment.origin.z) / segment.diff.z;
	const float tzMax = (aabb.max.z - segment.origin.z) / segment.diff.z;

	const float tNearX = (std::min)(txMin, txMax);
	const float tNearY = (std::min)(tyMin, tyMax);
	const float tNearZ = (std::min)(tzMin, tzMax);
	const float tFarX = (std::max)(txMin, txMax);
	const float tFarY = (std::max)(tyMin, tyMax);
	const float tFarZ = (std::max)(tzMin, tzMax);
	const float tMin = (std::max)((std::max)(tNearX, tNearY), tNearZ);
	const float tMax = (std::min)((std::min)(tFarX, tFarY), tFarZ);
	return tMin <= tMax && tMax >= 0.0f && tMin <= 1.0f;
}

bool IsCollision(const AABB& aabb, const Line& line) {
	const float txMin = (aabb.min.x - line.origin.x) / line.diff.x;
	const float txMax = (aabb.max.x - line.origin.x) / line.diff.x;
	const float tyMin = (aabb.min.y - line.origin.y) / line.diff.y;
	const float tyMax = (aabb.max.y - line.origin.y) / line.diff.y;
	const float tzMin = (aabb.min.z - line.origin.z) / line.diff.z;
	const float tzMax = (aabb.max.z - line.origin.z) / line.diff.z;

	const float tNearX = (std::min)(txMin, txMax);
	const float tNearY = (std::min)(tyMin, tyMax);
	const float tNearZ = (std::min)(tzMin, tzMax);
	const float tFarX = (std::max)(txMin, txMax);
	const float tFarY = (std::max)(tyMin, tyMax);
	const float tFarZ = (std::max)(tzMin, tzMax);
	const float tMin = (std::max)((std::max)(tNearX, tNearY), tNearZ);
	const float tMax = (std::min)((std::min)(tFarX, tFarY), tFarZ);
	return tMin <= tMax;
}

bool IsCollision(const AABB& aabb, const Ray& ray) {
	const float txMin = (aabb.min.x - ray.origin.x) / ray.diff.x;
	const float txMax = (aabb.max.x - ray.origin.x) / ray.diff.x;
	const float tyMin = (aabb.min.y - ray.origin.y) / ray.diff.y;
	const float tyMax = (aabb.max.y - ray.origin.y) / ray.diff.y;
	const float tzMin = (aabb.min.z - ray.origin.z) / ray.diff.z;
	const float tzMax = (aabb.max.z - ray.origin.z) / ray.diff.z;

	const float tNearX = (std::min)(txMin, txMax);
	const float tNearY = (std::min)(tyMin, tyMax);
	const float tNearZ = (std::min)(tzMin, tzMax);
	const float tFarX = (std::max)(txMin, txMax);
	const float tFarY = (std::max)(tyMin, tyMax);
	const float tFarZ = (std::max)(tzMin, tzMax);
	const float tMin = (std::max)((std::max)(tNearX, tNearY), tNearZ);
	const float tMax = (std::min)((std::min)(tFarX, tFarY), tFarZ);
	return tMin <= tMax && tMax >= 0.0f;
}

bool IsCollision(const OBB& obb, const Sphere& sphere) {
	const Matrix4x4 inverseWorldMatrix = Matrix4x4::Inverse(obb.GetWorldMatrix());
	const Vector3 localCenter = Vector3::Transform(sphere.center, inverseWorldMatrix);
	const AABB localAABB{.min = -obb.size, .max = obb.size};
	const Sphere localSphere{localCenter, sphere.radius};
	return IsCollision(localAABB, localSphere);
}

bool IsCollision(const OBB& obb, const Segment& segment) {
	const Matrix4x4 inverseWorldMatrix = Matrix4x4::Inverse(obb.GetWorldMatrix());
	const Vector3 localOrigin = Vector3::Transform(segment.origin, inverseWorldMatrix);
	const Vector3 localEnd = Vector3::Transform(segment.origin + segment.diff, inverseWorldMatrix);
	const AABB localAABB{-obb.size, obb.size};
	const Segment localSegment{localOrigin, localEnd - localOrigin};
	return IsCollision(localAABB, localSegment);
}

bool IsCollision(const OBB& obb, const Line& line) {
	const Matrix4x4 inverseWorldMatrix = Matrix4x4::Inverse(obb.GetWorldMatrix());
	const Vector3 localOrigin = Vector3::Transform(line.origin, inverseWorldMatrix);
	const Vector3 localEnd = Vector3::Transform(line.origin + line.diff, inverseWorldMatrix);
	const AABB localAABB{-obb.size, obb.size};
	const Line localLine{localOrigin, localEnd - localOrigin};
	return IsCollision(localAABB, localLine);
}

bool IsCollision(const OBB& obb, const Ray& ray) {
	const Matrix4x4 inverseWorldMatrix = Matrix4x4::Inverse(obb.GetWorldMatrix());
	const Vector3 localOrigin = Vector3::Transform(ray.origin, inverseWorldMatrix);
	const Vector3 localEnd = Vector3::Transform(ray.origin + ray.diff, inverseWorldMatrix);
	const AABB localAABB{-obb.size, obb.size};
	const Ray localRay{localOrigin, localEnd - localOrigin};
	return IsCollision(localAABB, localRay);
}

bool IsCollision(const OBB& obb1, const OBB& obb2) {
	for (const Vector3& axis : obb1.orientations) {
		if (!IsOverlappingOnAxis(obb1, obb2, axis)) {
			return false;
		}
	}
	for (const Vector3& axis : obb2.orientations) {
		if (!IsOverlappingOnAxis(obb1, obb2, axis)) {
			return false;
		}
	}
	for (const Vector3& axis1 : obb1.orientations) {
		for (const Vector3& axis2 : obb2.orientations) {
			const Vector3 crossAxis = Vector3::Cross(axis1, axis2);
			if (!IsOverlappingOnAxis(obb1, obb2, crossAxis)) {
				return false;
			}
		}
	}
	return true;
}
