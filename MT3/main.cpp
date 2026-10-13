#include <Novice.h>
#define _USE_MATH_DEFINES
#include "Collision.h"
#include "Draw.h"
#include "GeometryTypes.h"
#include "Matrix4x4.h"
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

	// MT4_00_02
	// ----------------------------------------------------
	const float deltaTime = 1.0f / 60.0f;

	const float playerRadius = 12.0f;
	Vector3 player{};

	const float followerRadius = 20.0f;
	Vector3 follower{640.0f, 360.0f, 0.0f};

	float speed = 3.0f;

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

		int mouseX = 0;
		int mouseY = 0;
		Novice::GetMousePosition(&mouseX, &mouseY);
		player = {static_cast<float>(mouseX), static_cast<float>(mouseY), 0.0f};

		follower += (player - follower) * (speed * deltaTime);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawLine(static_cast<int>(follower.x), static_cast<int>(follower.y), static_cast<int>(player.x), static_cast<int>(player.y), WHITE);
		Novice::DrawEllipse(static_cast<int>(player.x), static_cast<int>(player.y), static_cast<int>(playerRadius), static_cast<int>(playerRadius), 0.0f, RED, kFillModeSolid);
		Novice::DrawEllipse(static_cast<int>(follower.x), static_cast<int>(follower.y), static_cast<int>(followerRadius), static_cast<int>(followerRadius), 0.0f, GREEN, kFillModeSolid);

		///
		/// ↑描画処理ここまで
		///

		///
		/// ↓ImGUIここから
		///

#ifdef _DEBUG

		ImGui::Begin("Interpolation Controller");

		ImGui::Text("Target: Mouse Position (Red Circle)");
		ImGui::Separator();
		ImGui::SliderFloat("Speed", &speed, 0.0f, 20.0f, "%.1f");
		ImGui::Text("Mouse Pos: (%.1f, %.1f)", player.x, player.y);
		ImGui::Text("Circle Pos: (%.1f, %.1f)", follower.x, follower.y);
		ImGui::Text("Distance to target: %.1f px", Length(player - follower));

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
