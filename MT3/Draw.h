#pragma once

#include "GeometryTypes.h"
#include "Matrix4x4.h"
#include "Vector3.h"

#include <cstdint>

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color);
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color);
void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color);
void DrawLine(const Vector3& v1, const Vector3& v2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color);
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color);
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color);
void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color);
void DrawBezier(
    const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix,
    std::uint32_t color);
