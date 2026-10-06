#include "Draw.h"

#include "MathUtility.h"

#include <Novice.h>

#include <cmath>
#include <numbers>

namespace {

Vector3 ToScreen(const Vector3& position, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	return Transform(Transform(position, viewProjectionMatrix), viewportMatrix);
}

void DrawBoxEdges(const Vector3 (&vertices)[8], std::uint32_t color) {
	constexpr int kEdgeIndices[12][2] = {
	    {0, 1}, {1, 2}, {2, 3}, {3, 0},
	    {4, 5}, {5, 6}, {6, 7}, {7, 4},
	    {0, 4}, {1, 5}, {2, 6}, {3, 7},
	};

	for (const auto& edge : kEdgeIndices) {
		const Vector3& start = vertices[edge[0]];
		const Vector3& end = vertices[edge[1]];
		Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), color);
	}
}

} // namespace

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	constexpr float kGridHalfWidth = 2.0f;
	constexpr std::uint32_t kSubdivision = 10;
	constexpr float kGridEvery = (kGridHalfWidth * 2.0f) / static_cast<float>(kSubdivision);

	for (std::uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		const float x = -kGridHalfWidth + static_cast<float>(xIndex) * kGridEvery;
		const Vector3 lineStart = ToScreen({x, 0.0f, -kGridHalfWidth}, viewProjectionMatrix, viewportMatrix);
		const Vector3 lineEnd = ToScreen({x, 0.0f, kGridHalfWidth}, viewProjectionMatrix, viewportMatrix);
		const std::uint32_t color = xIndex == kSubdivision / 2 ? BLACK : 0xAAAAAAFF;
		Novice::DrawLine(static_cast<int>(lineStart.x), static_cast<int>(lineStart.y), static_cast<int>(lineEnd.x), static_cast<int>(lineEnd.y), color);
	}

	for (std::uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		const float z = -kGridHalfWidth + static_cast<float>(zIndex) * kGridEvery;
		const Vector3 lineStart = ToScreen({-kGridHalfWidth, 0.0f, z}, viewProjectionMatrix, viewportMatrix);
		const Vector3 lineEnd = ToScreen({kGridHalfWidth, 0.0f, z}, viewProjectionMatrix, viewportMatrix);
		const std::uint32_t color = zIndex == kSubdivision / 2 ? BLACK : 0xAAAAAAFF;
		Novice::DrawLine(static_cast<int>(lineStart.x), static_cast<int>(lineStart.y), static_cast<int>(lineEnd.x), static_cast<int>(lineEnd.y), color);
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color) {
	constexpr std::uint32_t kSubdivision = 12;
	constexpr float kPi = std::numbers::pi_v<float>;
	constexpr float kLonEvery = 2.0f * kPi / static_cast<float>(kSubdivision);
	constexpr float kLatEvery = kPi / static_cast<float>(kSubdivision);

	for (std::uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		const float lat = -kPi / 2.0f + kLatEvery * static_cast<float>(latIndex);
		for (std::uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			const float lon = static_cast<float>(lonIndex) * kLonEvery;
			const Vector3 a = ToScreen(
			    {sphere.radius * std::cos(lat) * std::cos(lon) + sphere.center.x, sphere.radius * std::sin(lat) + sphere.center.y,
			     sphere.radius * std::cos(lat) * std::sin(lon) + sphere.center.z},
			    viewProjectionMatrix, viewportMatrix);
			const Vector3 b = ToScreen(
			    {sphere.radius * std::cos(lat + kLatEvery) * std::cos(lon) + sphere.center.x, sphere.radius * std::sin(lat + kLatEvery) + sphere.center.y,
			     sphere.radius * std::cos(lat + kLatEvery) * std::sin(lon) + sphere.center.z},
			    viewProjectionMatrix, viewportMatrix);
			const Vector3 c = ToScreen(
			    {sphere.radius * std::cos(lat) * std::cos(lon + kLonEvery) + sphere.center.x, sphere.radius * std::sin(lat) + sphere.center.y,
			     sphere.radius * std::cos(lat) * std::sin(lon + kLonEvery) + sphere.center.z},
			    viewProjectionMatrix, viewportMatrix);

			Novice::DrawLine(static_cast<int>(a.x), static_cast<int>(a.y), static_cast<int>(b.x), static_cast<int>(b.y), color);
			Novice::DrawLine(static_cast<int>(a.x), static_cast<int>(a.y), static_cast<int>(c.x), static_cast<int>(c.y), color);
		}
	}
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color) {
	const Vector3 center = plane.normal * plane.distance;
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = -perpendiculars[0];
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = -perpendiculars[2];

	Vector3 points[4];
	for (int index = 0; index < 4; ++index) {
		points[index] = ToScreen(center + perpendiculars[index] * 2.0f, viewProjectionMatrix, viewportMatrix);
	}
	Novice::DrawLine(static_cast<int>(points[0].x), static_cast<int>(points[0].y), static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[2].x), static_cast<int>(points[2].y), static_cast<int>(points[1].x), static_cast<int>(points[1].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y), static_cast<int>(points[3].x), static_cast<int>(points[3].y), color);
	Novice::DrawLine(static_cast<int>(points[3].x), static_cast<int>(points[3].y), static_cast<int>(points[0].x), static_cast<int>(points[0].y), color);
}

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color) {
	DrawLine(segment.origin, segment.origin + segment.diff, viewProjectionMatrix, viewportMatrix, color);
}

void DrawLine(const Vector3& v1, const Vector3& v2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color) {
	const Vector3 start = ToScreen(v1, viewProjectionMatrix, viewportMatrix);
	const Vector3 end = ToScreen(v2, viewProjectionMatrix, viewportMatrix);
	Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), color);
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color) {
	const Vector3 v0 = ToScreen(triangle.vertices[0], viewProjectionMatrix, viewportMatrix);
	const Vector3 v1 = ToScreen(triangle.vertices[1], viewProjectionMatrix, viewportMatrix);
	const Vector3 v2 = ToScreen(triangle.vertices[2], viewProjectionMatrix, viewportMatrix);
	Novice::DrawTriangle(
	    static_cast<int>(v0.x), static_cast<int>(v0.y), static_cast<int>(v1.x), static_cast<int>(v1.y), static_cast<int>(v2.x), static_cast<int>(v2.y), color, kFillModeWireFrame);
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color) {
	Vector3 vertices[8] = {
	    {aabb.min.x, aabb.min.y, aabb.min.z}, {aabb.max.x, aabb.min.y, aabb.min.z}, {aabb.max.x, aabb.max.y, aabb.min.z}, {aabb.min.x, aabb.max.y, aabb.min.z},
	    {aabb.min.x, aabb.min.y, aabb.max.z}, {aabb.max.x, aabb.min.y, aabb.max.z}, {aabb.max.x, aabb.max.y, aabb.max.z}, {aabb.min.x, aabb.max.y, aabb.max.z},
	};
	for (Vector3& vertex : vertices) {
		vertex = ToScreen(vertex, viewProjectionMatrix, viewportMatrix);
	}
	DrawBoxEdges(vertices, color);
}

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, std::uint32_t color) {
	Vector3 vertices[8] = {
	    {-obb.size.x, -obb.size.y, -obb.size.z}, {obb.size.x, -obb.size.y, -obb.size.z}, {obb.size.x, obb.size.y, -obb.size.z}, {-obb.size.x, obb.size.y, -obb.size.z},
	    {-obb.size.x, -obb.size.y, obb.size.z},  {obb.size.x, -obb.size.y, obb.size.z},  {obb.size.x, obb.size.y, obb.size.z},  {-obb.size.x, obb.size.y, obb.size.z},
	};
	const Matrix4x4 worldMatrix = obb.GetWorldMatrix();
	for (Vector3& vertex : vertices) {
		vertex = Transform(vertex, worldMatrix);
		vertex = ToScreen(vertex, viewProjectionMatrix, viewportMatrix);
	}
	DrawBoxEdges(vertices, color);
}

void DrawBezier(
    const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix,
    std::uint32_t color) {
	constexpr int kSegments = 32;
	for (int index = 0; index < kSegments; ++index) {
		const float t0 = static_cast<float>(index) / static_cast<float>(kSegments);
		const float t1 = static_cast<float>(index + 1) / static_cast<float>(kSegments);
		DrawLine(Bezier(controlPoint0, controlPoint1, controlPoint2, t0), Bezier(controlPoint0, controlPoint1, controlPoint2, t1), viewProjectionMatrix, viewportMatrix, color);
	}
}
