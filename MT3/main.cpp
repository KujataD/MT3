#include <Novice.h>
#define _USE_MATH_DEFINES
#include "Collision.h"
#include "Draw.h"
#include "GeometryTypes.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2B_04_オオツカ_ダイチ_MT3";

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// カメラ
	// ----------------------------------------------------

	Vector3 cameraTranslate{0.0f, 2.0f, -6.49f};
	Vector3 cameraRotate{0.21f, 0.0f, 0.0f};

	int mouseX = 0;
	int mouseY = 0;
	int prevMouseX = 0;
	int prevMouseY = 0;
	const float kRotateSpeed = 0.0025f;
	const float kMoveSpeed = 0.1f;

	// 課題02_08
	// ----------------------------------------------------
	Vector3 rotate1{0.0f, 0.0f, 0.0f};
	Vector3 rotate2{-0.05f, -2.49f, 0.15f};
	OBB obb1{
	    .center = {0.0f,               0.0f,               0.0f              },
          .orientations = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
          .size = {0.83f,              0.26f,              0.24f             }
    };
	OBB obb2{
	    .center = {0.9f,               0.66f,              0.78f             },
          .orientations = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
          .size = {0.5f,               0.37f,              0.5f              }
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

		obb1.UpdateOBBOrientations(rotate1);
		obb2.UpdateOBBOrientations(rotate2);

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

		if (IsCollision(obb1, obb2)) {
			DrawOBB(obb1, viewProjectionMatrix, viewportMatrix, RED);
		} else {
			DrawOBB(obb1, viewProjectionMatrix, viewportMatrix, WHITE);
		}
		DrawOBB(obb2, viewProjectionMatrix, viewportMatrix, WHITE);

		///
		/// ↑描画処理ここまで
		///

		///
		/// ↓ImGUIここから
		///

#ifdef _DEBUG

		ImGui::Begin("Window");

		ImGui::DragFloat3("obb1.center", &obb1.center.x, 0.01f);
		ImGui::DragFloat3("obb1.rotate", &rotate1.x, 0.01f);
		ImGui::DragFloat3("obb1.orientations[0]", &obb1.orientations[0].x, 0.01f);
		ImGui::DragFloat3("obb1.orientations[1]", &obb1.orientations[1].x, 0.01f);
		ImGui::DragFloat3("obb1.orientations[2]", &obb1.orientations[2].x, 0.01f);
		ImGui::DragFloat3("obb1.size", &obb1.size.x, 0.01f);

		ImGui::DragFloat3("obb2.center", &obb2.center.x, 0.01f);
		ImGui::DragFloat3("obb2.rotate", &rotate2.x, 0.01f);
		ImGui::DragFloat3("obb2.orientations[0]", &obb2.orientations[0].x, 0.01f);
		ImGui::DragFloat3("obb2.orientations[1]", &obb2.orientations[1].x, 0.01f);
		ImGui::DragFloat3("obb2.orientations[2]", &obb2.orientations[2].x, 0.01f);
		ImGui::DragFloat3("obb2.size", &obb2.size.x, 0.01f);

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
