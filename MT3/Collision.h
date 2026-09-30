#pragma once

#include "GeometryTypes.h"
#include "Vector3.h"

bool IsCollision(const Sphere& sphere, const Plane& plane);
bool IsCollision(const Segment& segment, const Plane& plane);
bool IsCollision(const Segment& segment, const Triangle& triangle);
bool IsCollision(const AABB& aabb1, const AABB& aabb2);
bool IsCollision(const AABB& aabb, const Sphere& sphere);
bool IsCollision(const AABB& aabb, const Segment& segment);
bool IsCollision(const AABB& aabb, const Line& line);
bool IsCollision(const AABB& aabb, const Ray& ray);
bool IsCollision(const OBB& obb, const Sphere& sphere);
bool IsCollision(const OBB& obb, const Segment& segment);
bool IsCollision(const OBB& obb, const Line& line);
bool IsCollision(const OBB& obb, const Ray& ray);
bool IsCollision(const OBB& obb1, const OBB& obb2);
