#include <Novice.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector3.h"
#include "Matrix4x4.h"

const char kWindowTitle[] = "LE2B_04_オオツカ_ダイチ_MT3";

static const int kRowHeight = 20;
static const int kColumnWidth = 66;
static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 計算
	// ----------------------------------------------------
	Vector3 v1{1.2f, -3.9f, 2.5f};
	Vector3 v2{2.8f, 0.4f, -1.3f};
	Vector3 cross = Vector3::Cross(v1, v2);

	Vector3 rotate{0.0f, 0.0f, 0.0f};
	Vector3 translate{0.0f, 0.0f, 0.0f};
	Vector3 cameraPosition{0.0f, 0.0f, -100.0f};
	Vector3 kLocalVertices[3];
	kLocalVertices[0] = {0.0f, 10.0f, 0.0f};
	kLocalVertices[1] = {10.0f, -10.f, 0.0f};
	kLocalVertices[2] = {-10.0f, -10.0f, 0.0f};

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

		// 三角形を動かす
		if (keys[DIK_W]) {
			translate.z += 1.0f;
		}
		if (keys[DIK_S]) {
			translate.z -= 1.0f;
		}
		if (keys[DIK_A]) {
			translate.x -= 1.0f;
		}
		if (keys[DIK_D]) {
			translate.x += 1.0f;
		}

		// 三角形の回転
		rotate.y += 0.05f;

		// 各種行列の計算
		Matrix4x4 worldMatrix = Matrix4x4::MakeAffineMatrix({1.0f, 1.0f, 1.0f}, rotate, translate);
		Matrix4x4 cameraMatrix = Matrix4x4::MakeAffineMatrix({1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, cameraPosition);
		Matrix4x4 viewMatrix = Matrix4x4:: Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = worldMatrix * viewMatrix * projectionMatrix;
		Matrix4x4 viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 screenVertices[3];
		
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Vector3::Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Vector3::Transform(ndcVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
	
		VectorScreenPrintf(0, 0, cross, "Cross");
		Novice::DrawTriangle(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid);

		///
		/// ↑描画処理ここまで
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