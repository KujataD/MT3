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


void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);


void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 計算
	// ----------------------------------------------------

	Vector3 cameraTranslate{0.0f, 2.0f, -6.49f};
	Vector3 cameraRotate{0.21f, 0.0f, 0.0f};

	Segment segment{
	    {-2.0f, -1.0f, 0.0f},
        {3.0f,  2.0f,  2.0f}
    };

	Vector3 point{-1.5f, 0.6f, 0.6f};
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

		Vector3 project = Vector3::Project(point - segment.origin, segment.diff);
		Vector3 closestPoint = Vector3::ClosestPoint(point, segment);

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

		Sphere pointSphere{point, 0.01f}; // 1cmの球を描画
		Sphere closestPointSphere{closestPoint, 0.01f};
		DrawSphere(pointSphere, viewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(closestPointSphere, viewProjectionMatrix, viewportMatrix, BLACK);

		Vector3 start = Vector3::Transform(Vector3::Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		Vector3 end = Vector3::Transform(Vector3::Transform(segment.origin + segment.diff, viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		///
		/// ↑描画処理ここまで
		///

		///
		/// ↓ImGUIここから
		///

#ifdef _DEBUG

		ImGui::Begin("Window");
		ImGui::DragFloat3("Point", &point.x, 0.1f);
		ImGui::DragFloat3("Segment origin", &segment.origin.x, 0.1f);
		ImGui::DragFloat3("Segment diff", &segment.diff.x, 0.1f);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();

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
