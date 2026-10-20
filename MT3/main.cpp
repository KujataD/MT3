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
	Quaternion q1 = {2.0f, 3.0f, 4.0f, 1.0f};
	Quaternion q2 = {1.0f, 3.0f, 5.0f, 2.0f};
	Quaternion identity = IdentityQuaternion();
	Quaternion conj = Conjugate(q1);
	Quaternion inv = Inverse(q1);
	Quaternion normal = Normalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = Multiply(q2, q1);
	float norm = Norm(q1);

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

		QuaternionScreenPrintf(0, kRowHeight * 0, identity, "   : Identity");
		QuaternionScreenPrintf(0, kRowHeight * 1, conj, "   : Conjugate");
		QuaternionScreenPrintf(0, kRowHeight * 2, inv, "   : Inverse");
		QuaternionScreenPrintf(0, kRowHeight * 3, normal, "   : Normalize");
		QuaternionScreenPrintf(0, kRowHeight * 4, mul1, "   : Multiply(q1, q2)");
		QuaternionScreenPrintf(0, kRowHeight * 5, mul2, "   : Multiply(q2, q1)");
		ScalarScreenPrintf(0, kRowHeight * 6, norm, "   : Norm");

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
