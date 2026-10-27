#include <Novice.h>
#define _USE_MATH_DEFINES
#include "Collision.h"
#include "Draw.h"
#include "GeometryTypes.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "ScreenPrint.h"
#include "Vector3.h"
#include <cmath>
#include <imgui.h>
#include <numbers>

const char kWindowTitle[] = "LE2B_03_オオツカ_ダイチ_MT4";

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// Quaternion
	// ----------------------------------------------------
	Quaternion rotation = MakeRotateAxisAngleQuaternion(Normalize(Vector3{1.0f, 0.4f, -0.2f}), 0.45f);
	Vector3 pointY = {2.1f, -0.9f, 1.3f};
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);

	const int kRowHeight = 20;

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

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		QuaternionScreenPrintf(0, kRowHeight * 0, rotation, "   : rotation");
		MatrixScreenPrintf(0, kRowHeight * 1, rotateMatrix, "rotateMatrix");
		VectorScreenPrintf(0, kRowHeight * 6, rotateByQuaternion, "   : rotateByQuaternion");
		VectorScreenPrintf(0, kRowHeight * 7, rotateByMatrix, "   : rotateByMatrix");

		///
		/// ↑描画処理ここまで
		///

		///
		/// ↓ImGUIここから
		///

#ifdef _DEBUG

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
