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

struct Spherical {
	float radius; // 動径 r 接近後退
	float theta;  // 仰角 θ 見下ろし角度
	float phi;    // 方位角 φ 旋回
};

Vector3 ToCartesian(const Spherical s) {
	float rho = s.radius * std::cos(s.theta);
	return {rho * std::cos(s.phi), s.radius * std::sin(s.theta), rho * std::sin(s.phi)};
}

Spherical ToSpherical(const Vector3& p) {
	float r = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	if (r == 0.0f) {
		return {0.0f, 0.0f, 0.0f};
	}

	float sinTheta = std::clamp(p.y / r, -1.0f, 1.0f);
	float phi = 0.0f;
	if (p.x != 0.0f || p.z != 0.0f) {
		phi = std ::atan2(p.z, p.x);
	}
	return {r, std::asin(sinTheta), phi};
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// MT4_00_01
	// ----------------------------------------------------
	float halfpi = std::numbers::pi_v<float> / 2.0f;
	Spherical s{6.0f, 0.0f, -halfpi};
	Vector3 target{};
	const Vector3 worldUp{0.0f, 1.0f, 0.0f};
	const float limit = halfpi - 0.01f;

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

		s.radius = std::max<float>(s.radius, 0.1f);
		s.theta = std::clamp(s.theta, -limit, limit);

		Vector3 eye = target + ToCartesian(s);

		Vector3 forward = Normalize(target, eye);
		Vector3 right = Normalize(Cross(worldUp, forward));
		Vector3 up = Cross(forward, right);
		const Vector3 orientations[3] = {right, up, forward};

		// 各種行列の計算
		Matrix4x4 cameraMatrix = MakeAffineMatrixOrientations(orientations, eye);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = viewMatrix * projectionMatrix;
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(viewProjectionMatrix, viewportMatrix);

		///
		/// ↑描画処理ここまで
		///

		///
		/// ↓ImGUIここから
		///

#ifdef _DEBUG

		ImGui::Begin("Spherical Coordinates");

		ImGui::Text("Target: (0, 0, 0) / +Y up / Camera +Z forward");
		ImGui::Separator();

		ImGui::InputFloat("Radius", &s.radius, 0.1f, 1.0f, "%.3f");
		ImGui::InputFloat("Theta: elevation (rad)", &s.theta, 0.01f, 0.1f, "%.3f");
		ImGui::InputFloat("Phi (rad)", &s.phi, 0.01f, 0.1f, "%.3f");
		ImGui::Separator();

		ImGui::Text("Spherical: r = %.3f, theta = %.3f rad, phi = %.3f rad", s.radius, s.theta, s.phi);
		ImGui::Text("Cartesian: x = %.3f, y = %.3f, z = %.3f", eye.x, eye.y, eye.z);
		ImGui::Separator();

		ImGui::Text("Camera matrix");
		for (int row = 0; row < 4; ++row) {
			ImGui::Text("%10.3f %10.3f %10.3f %10.3f", cameraMatrix.m[row][0], cameraMatrix.m[row][1], cameraMatrix.m[row][2], cameraMatrix.m[row][3]);
		}

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
