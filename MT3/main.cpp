#include <Novice.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2B_04_オオツカ_ダイチ_MT3";

static const int kRowHeight = 20;
static const int kColumnWidth = 66;
static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

struct Triangle {
	Vector3 vertices[3];
};

struct AABB {
	Vector3 min;
	Vector3 max;
};

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

bool IsCollision(const Sphere& sphere, const Plane& plane);

bool IsCollision(const Segment& line, const Plane& plane);

bool IsCollision(const Segment& segment, const Triangle& triangle);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

void DrawLine(const Segment& line, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 計算
	// ----------------------------------------------------

	Vector3 cameraTranslate{0.0f, 2.0f, -6.49f};
	Vector3 cameraRotate{0.21f, 0.0f, 0.0f};

	int mouseX = 0;
	int mouseY = 0;
	int prevMouseX = 0;
	int prevMouseY = 0;
	const float kRotateSpeed = 0.0025f;
	const float kMoveSpeed = 0.1f;

	Plane plane;
	plane.normal = {0.0f, 1.0f, 0.0f};
	plane.distance = 1.0f;

	Segment segment;
	segment.origin = {-0.5f, 0.5f, 0.8f};
	segment.diff = {1.0f, 0.5f, 0.8f};

	Triangle triangle;
	triangle.vertices[0] = {0.0f, 1.0f, 0.0f};
	triangle.vertices[1] = {1.0f, 0.0f, 0.0f};
	triangle.vertices[2] = {-1.0f, 0.0f, 0.0f};

	AABB aabb1{
	    .min{-0.5f, -0.5f, -0.5f}, 
		.max{0.0f, 0.0f, 0.0f},
    };

	AABB aabb2{
	    .min{0.2f, 0.2f, 0.2f},
	    .max{1.0f, 1.0f, 1.0f},
	};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		{ // カメラ操作
			prevMouseX = mouseX;
			prevMouseY = mouseY;
			Novice::GetMousePosition(&mouseX, &mouseY);

			if (Novice::IsPressMouse(1)) {
				int dx = mouseX - prevMouseX;
				int dy = mouseY - prevMouseY;

				cameraRotate.y += dx * kRotateSpeed;
				cameraRotate.x += dy * kRotateSpeed;
			}

			Vector3 forward{sinf(cameraRotate.y), 0, cosf(cameraRotate.y)};
			Vector3 right{forward.z, 0, -forward.x};

			if (keys[DIK_W]) {
				cameraTranslate = cameraTranslate + forward * kMoveSpeed;
			}
			if (keys[DIK_S]) {
				cameraTranslate = cameraTranslate - forward * kMoveSpeed;
			}
			if (keys[DIK_A]) {
				cameraTranslate = cameraTranslate - right * kMoveSpeed;
			}
			if (keys[DIK_D]) {
				cameraTranslate = cameraTranslate + right * kMoveSpeed;
			}
			if (keys[DIK_Q]) {
				cameraTranslate.y += kMoveSpeed;
			}
			if (keys[DIK_E]) {
				cameraTranslate.y -= kMoveSpeed;
			}
		}

		// 各種行列の計算
		Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix({1.0f, 1.0f, 1.0f}, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Matrix4x4::Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = viewMatrix * projectionMatrix;
		Matrix4x4 viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);
		
		if (IsCollision(segment, triangle)) {
			DrawLine(segment, viewProjectionMatrix, viewportMatrix, RED);
		} else {
			DrawLine(segment, viewProjectionMatrix, viewportMatrix, WHITE);
		}

		DrawTriangle(triangle,viewProjectionMatrix, viewportMatrix, WHITE);

		///
		/// ↑描画処理ここまで
		///

		///
		/// ↓ImGUIここから
		///

#ifdef _DEBUG

		ImGui::Begin("Window");
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("Plane.Distance", &plane.distance, 0.01f);
		ImGui::DragFloat3("Segment.Origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Segment.Diff", &segment.diff.x, 0.01f);
		ImGui::End();

		Novice::ScreenPrintf(10, 10, "Mouse Right Drag : Camera Rotate");
		Novice::ScreenPrintf(10, 30, "WASDQE : MoveCamera");

#endif // _DEBUG

		///
		/// ↑ImGUIここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		Vector3 lineStart = {-kGridHalfWidth + xIndex * kGridEvery, 0.0f, -kGridHalfWidth};
		Vector3 lineEnd = {-kGridHalfWidth + xIndex * kGridEvery, 0.0f, kGridHalfWidth};

		lineStart = Vector3::Transform(lineStart, viewProjectionMatrix);
		lineEnd = Vector3::Transform(lineEnd, viewProjectionMatrix);

		lineStart = Vector3::Transform(lineStart, viewportMatrix);
		lineEnd = Vector3::Transform(lineEnd, viewportMatrix);

		if (xIndex == static_cast<int>(kSubdivision / 2.0f)) {
			Novice::DrawLine(static_cast<int>(lineStart.x), static_cast<int>(lineStart.y), static_cast<int>(lineEnd.x), static_cast<int>(lineEnd.y), BLACK);
		} else {
			Novice::DrawLine(static_cast<int>(lineStart.x), static_cast<int>(lineStart.y), static_cast<int>(lineEnd.x), static_cast<int>(lineEnd.y), 0xAAAAAAFF);
		}
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		Vector3 lineStart = {-kGridHalfWidth, 0.0f, -kGridHalfWidth + zIndex * kGridEvery};
		Vector3 lineEnd = {kGridHalfWidth, 0.0f, -kGridHalfWidth + zIndex * kGridEvery};

		lineStart = Vector3::Transform(lineStart, viewProjectionMatrix);
		lineEnd = Vector3::Transform(lineEnd, viewProjectionMatrix);

		lineStart = Vector3::Transform(lineStart, viewportMatrix);
		lineEnd = Vector3::Transform(lineEnd, viewportMatrix);

		if (zIndex == static_cast<int>(kSubdivision / 2.0f)) {
			Novice::DrawLine(static_cast<int>(lineStart.x), static_cast<int>(lineStart.y), static_cast<int>(lineEnd.x), static_cast<int>(lineEnd.y), BLACK);
		} else {
			Novice::DrawLine(static_cast<int>(lineStart.x), static_cast<int>(lineStart.y), static_cast<int>(lineEnd.x), static_cast<int>(lineEnd.y), 0xAAAAAAFF);
		}
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 12;
	const float kLonEvery = static_cast<float>(2.0f * M_PI / kSubdivision);
	const float kLatEvery = static_cast<float>(M_PI / kSubdivision);

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = static_cast<float>(-M_PI / 2.0f + kLatEvery * latIndex); // 緯度

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 経度

			Vector3 a, b, c;

			a.x = sphere.radius * cosf(lat) * cosf(lon) + sphere.center.x;
			a.y = sphere.radius * sinf(lat) + sphere.center.y;
			a.z = sphere.radius * cosf(lat) * sinf(lon) + sphere.center.z;

			b.x = sphere.radius * cosf(lat + kLatEvery) * cosf(lon) + sphere.center.x;
			b.y = sphere.radius * sinf(lat + kLatEvery) + sphere.center.y;
			b.z = sphere.radius * cosf(lat + kLatEvery) * sinf(lon) + sphere.center.z;

			c.x = sphere.radius * cosf(lat) * cosf(lon + kLonEvery) + sphere.center.x;
			c.y = sphere.radius * sinf(lat) + sphere.center.y;
			c.z = sphere.radius * cosf(lat) * sinf(lon + kLonEvery) + sphere.center.z;

			a = Vector3::Transform(a, viewProjectionMatrix);
			b = Vector3::Transform(b, viewProjectionMatrix);
			c = Vector3::Transform(c, viewProjectionMatrix);
			a = Vector3::Transform(a, viewportMatrix);
			b = Vector3::Transform(b, viewportMatrix);
			c = Vector3::Transform(c, viewportMatrix);

			Novice::DrawLine(static_cast<int>(a.x), static_cast<int>(a.y), static_cast<int>(b.x), static_cast<int>(b.y), color);
			Novice::DrawLine(static_cast<int>(a.x), static_cast<int>(a.y), static_cast<int>(c.x), static_cast<int>(c.y), color);
		}
	}
}

bool IsCollision(const Sphere& sphere, const Plane& plane) { 
	float k = std::abs(Vector3::Dot(plane.normal, sphere.center) - plane.distance);
	return (k <= sphere.radius);
}

bool IsCollision(const Segment& segment, const Plane& plane) {
	float dot = Vector3::Dot(plane.normal, segment.diff);

	if (dot == 0.0f) {
		return false;
	}
	
	float t = (plane.distance - Vector3::Dot(segment.origin, plane.normal)) / dot;

	return (t >= 0.0f && t <= 1.0f);
}

bool IsCollision(const Segment& segment, const Triangle& triangle) {
	Vector3 normal = Vector3::Normalize(Vector3::Cross(triangle.vertices[1] - triangle.vertices[0], triangle.vertices[2] - triangle.vertices[0]));

	float dot = Vector3::Dot(normal, segment.diff);
	
	if (dot == 0.0f) {
		return false;
	}

	float d = Vector3::Dot(normal, triangle.vertices[0]);
	float t = (d - Vector3::Dot(segment.origin, normal)) / dot;

	Vector3 p = segment.origin + segment.diff * t;
    
	Vector3 v01 = triangle.vertices[1] - triangle.vertices[0];
	Vector3 v12 = triangle.vertices[2] - triangle.vertices[1];
	Vector3 v20 = triangle.vertices[0] - triangle.vertices[2];
	Vector3 v0p = p - triangle.vertices[0];
	Vector3 v1p = p - triangle.vertices[1];
	Vector3 v2p = p - triangle.vertices[2];
	Vector3 cross01 = Vector3::Cross(v01, v1p);
	Vector3 cross12 = Vector3::Cross(v12, v2p);
	Vector3 cross20 = Vector3::Cross(v20, v0p);

	bool resultA = (Vector3::Dot(cross01, normal) >= 0.0f && Vector3::Dot(cross12, normal) >= 0.0f && Vector3::Dot(cross20, normal) >= 0.0f);
	bool resultB = (t >= 0.0f && t <= 1.0f);

	return (resultA && resultB);
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = plane.normal * plane.distance;
	Vector3 perpendiculars[4];
	perpendiculars[0] = Vector3::Normalize(Vector3::Perpendicular(plane.normal));
	perpendiculars[1] = {-perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z};
	perpendiculars[2] = Vector3::Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = {-perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z}; 
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = perpendiculars[index] * 2.0f;
		Vector3 point = center + extend;
		points[index] = Vector3::Transform(Vector3::Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	Novice::DrawLine(static_cast<int>(points[0].x), static_cast<int>(points[0].y), static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[2].x), static_cast<int>(points[2].y), static_cast<int>(points[1].x), static_cast<int>(points[1].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y), static_cast<int>(points[3].x), static_cast<int>(points[3].y), color);
	Novice::DrawLine(static_cast<int>(points[3].x), static_cast<int>(points[3].y), static_cast<int>(points[0].x), static_cast<int>(points[0].y), color);
}

void DrawLine(const Segment& line, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 start = Vector3::Transform(Vector3::Transform(line.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Vector3::Transform(Vector3::Transform(line.origin + line.diff, viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), color);
}


void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 v0 = Vector3::Transform(Vector3::Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	Vector3 v1 = Vector3::Transform(Vector3::Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	Vector3 v2 = Vector3::Transform(Vector3::Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);

	Novice::DrawTriangle(
		static_cast<int>(v0.x), static_cast<int>(v0.y),
		static_cast<int>(v1.x), static_cast<int>(v1.y),
		static_cast<int>(v2.x), static_cast<int>(v2.y),
		color, kFillModeWireFrame);
}