#include <Novice.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "LE2B_07_オオツカ_ダイチ_MT3";

static const int kRowHeight = 20;
static const int kColumnWidth = 66;

struct Matrix4x4 {
	float m[4][4];
};

struct Vector3 {
	float x, y, z;
};

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
Matrix4x4 MakeScaleMatrix(const Vector3 scale);
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

Vector3 Add(const Vector3& v1, const Vector3& v2);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Vector3 Multiply(float scalar, const Vector3& v);
float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);
Vector3 Normalize(const Vector3& v);

Vector3 operator+(const Vector3& v, float scalar);
Vector3 operator+(const Vector3& v1, const Vector3& v2);
void operator+=(Vector3& v1, const Vector3& v2);

Vector3 operator-(const Vector3& v, float scalar);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
void operator-=(Vector3& v1, const Vector3& v2);

Vector3 operator*(const Vector3& v, float scalar);

Vector3 operator/(const Vector3& v, float scalar);

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Inverse(const Matrix4x4& m);
Matrix4x4 Transpose(const Matrix4x4& m);
Matrix4x4 MakeIdentity4x4();

Matrix4x4 operator/(const Matrix4x4& m, float scalar);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 計算
	// ----------------------------------------------------
	Vector3 translate{4.1f, 2.6f, 0.8f};
	Vector3 scale{1.5f, 5.2f, 7.3f};
	
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	
	Vector3 point{2.3f, 3.8f, 1.4f};
	
	Matrix4x4 transformMatrix = {
		1.0f, 2.0f, 3.0f, 4.0f,
		3.0f, 1.0f, 1.0f, 2.0f,
		1.0f, 4.0f, 2.0f, 3.0f,
		2.0f, 2.0f, 1.0f, 3.0f
	};
	
	Vector3 transformed = Transform(point, transformMatrix);

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

		VectorScreenPrintf(0, 0, transformed, "transformed");
		MatrixScreenPrintf(0, kRowHeight, translateMatrix, "translateMatrix");
		MatrixScreenPrintf(0, kRowHeight * 6, scaleMatrix, "scaleMatrix");

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

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) { 
	return {
	    {
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{translate.x, translate.y, translate.z, 1.0f}
		}
    }; 
}

Matrix4x4 MakeScaleMatrix(const Vector3 scale) {
	return {
	    {
			{scale.x, 0.0f, 0.0f, 0.0f}, 
			{0.0f, scale.y, 0.0f, 0.0f}, 
			{0.0f, 0.0f, scale.z, 0.0f}, 
			{0.0f, 0.0f, 0.0f,    1.0f}  
		}
    };
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

Vector3 Add(const Vector3& v1, const Vector3& v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }
Vector3 Subtract(const Vector3& v1, const Vector3& v2) { return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }
Vector3 Multiply(float scalar, const Vector3& v) { return {scalar * v.x, scalar * v.y, scalar * v.z}; }
float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
float Length(const Vector3& v) { return sqrtf(Dot(v, v)); }
Vector3 Normalize(const Vector3& v) { return v / Length(v); }

Vector3 operator+(const Vector3& v, float scalar) { return {v.x + scalar, v.y + scalar, v.z + scalar}; }
Vector3 operator+(const Vector3& v1, const Vector3& v2) { return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }
void operator+=(Vector3& v1, const Vector3& v2) { v1 = v1 + v2; }

Vector3 operator-(const Vector3& v, float scalar) { return {v.x - scalar, v.y - scalar, v.z - scalar}; }
Vector3 operator-(const Vector3& v1, const Vector3& v2) { return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }
void operator-=(Vector3& v1, const Vector3& v2) { v1 = v1 - v2; }

Vector3 operator*(const Vector3& v, float scalar) { return {v.x * scalar, v.y * scalar, v.z * scalar}; }

Vector3 operator/(const Vector3& v, float scalar) { return {v.x / scalar, v.y / scalar, v.z / scalar}; }

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	return {
	    {
         {
	            m1.m[0][0] + m2.m[0][0],
	            m1.m[0][1] + m2.m[0][1],
	            m1.m[0][2] + m2.m[0][2],
	            m1.m[0][3] + m2.m[0][3],
	        }, {
	            m1.m[1][0] + m2.m[1][0],
	            m1.m[1][1] + m2.m[1][1],
	            m1.m[1][2] + m2.m[1][2],
	            m1.m[1][3] + m2.m[1][3],
	        }, {
	            m1.m[2][0] + m2.m[2][0],
	            m1.m[2][1] + m2.m[2][1],
	            m1.m[2][2] + m2.m[2][2],
	            m1.m[2][3] + m2.m[2][3],
	        }, {
	            m1.m[3][0] + m2.m[3][0],
	            m1.m[3][1] + m2.m[3][1],
	            m1.m[3][2] + m2.m[3][2],
	            m1.m[3][3] + m2.m[3][3],
	        }, }
    };
}

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	return {
	    {
         {
	            m1.m[0][0] - m2.m[0][0],
	            m1.m[0][1] - m2.m[0][1],
	            m1.m[0][2] - m2.m[0][2],
	            m1.m[0][3] - m2.m[0][3],
	        }, {
	            m1.m[1][0] - m2.m[1][0],
	            m1.m[1][1] - m2.m[1][1],
	            m1.m[1][2] - m2.m[1][2],
	            m1.m[1][3] - m2.m[1][3],
	        }, {
	            m1.m[2][0] - m2.m[2][0],
	            m1.m[2][1] - m2.m[2][1],
	            m1.m[2][2] - m2.m[2][2],
	            m1.m[2][3] - m2.m[2][3],
	        }, {
	            m1.m[3][0] - m2.m[3][0],
	            m1.m[3][1] - m2.m[3][1],
	            m1.m[3][2] - m2.m[3][2],
	            m1.m[3][3] - m2.m[3][3],
	        }, }
    };
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	return {
	    {
         {
	            m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0],
	            m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1],
	            m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2],
	            m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3],
	        }, {m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0],
	         m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1],
	         m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2],
	         m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3]},
         {
	            m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0],
	            m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1],
	            m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2],
	            m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3],
	        }, {
	            m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0],
	            m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1],
	            m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2],
	            m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3],
	        }, }
    };
}

Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result;

	result.m[0][0] = m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][3] * m.m[3][2] - m.m[1][2] * m.m[2][1] * m.m[3][3] -
	                 m.m[1][3] * m.m[2][2] * m.m[3][1];

	result.m[0][1] = -m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][3] * m.m[3][2] +
	                 m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][3] * m.m[2][2] * m.m[3][1];

	result.m[0][2] = m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][3] * m.m[3][2] - m.m[0][2] * m.m[1][1] * m.m[3][3] -
	                 m.m[0][3] * m.m[1][2] * m.m[3][1];

	result.m[0][3] = -m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][3] * m.m[2][2] +
	                 m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][3] * m.m[1][2] * m.m[2][1];

	result.m[1][0] = -m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][3] * m.m[3][2] +
	                 m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][3] * m.m[2][2] * m.m[3][0];

	result.m[1][1] = m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][3] * m.m[3][2] - m.m[0][2] * m.m[2][0] * m.m[3][3] -
	                 m.m[0][3] * m.m[2][2] * m.m[3][0];

	result.m[1][2] = -m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][3] * m.m[3][2] +
	                 m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][3] * m.m[1][2] * m.m[3][0];

	result.m[1][3] = m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][3] * m.m[2][2] - m.m[0][2] * m.m[1][0] * m.m[2][3] -
	                 m.m[0][3] * m.m[1][2] * m.m[2][0];

	result.m[2][0] = m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[1][1] * m.m[2][0] * m.m[3][3] -
	                 m.m[1][3] * m.m[2][1] * m.m[3][0];

	result.m[2][1] = -m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][0] * m.m[2][3] * m.m[3][1] +
	                 m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][3] * m.m[2][1] * m.m[3][0];

	result.m[2][2] = m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][0] * m.m[1][3] * m.m[3][1] - m.m[0][1] * m.m[1][0] * m.m[3][3] -
	                 m.m[0][3] * m.m[1][1] * m.m[3][0];

	result.m[2][3] = -m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] +
	                 m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][3] * m.m[1][1] * m.m[2][0];

	result.m[3][0] = -m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][0] * m.m[2][2] * m.m[3][1] +
	                 m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][2] * m.m[2][1] * m.m[3][0];

	result.m[3][1] = m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][0] * m.m[2][2] * m.m[3][1] - m.m[0][1] * m.m[2][0] * m.m[3][2] -
	                 m.m[0][2] * m.m[2][1] * m.m[3][0];

	result.m[3][2] = -m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][0] * m.m[1][2] * m.m[3][1] +
	                 m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][2] * m.m[1][1] * m.m[3][0];

	result.m[3][3] = m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] - m.m[0][1] * m.m[1][0] * m.m[2][2] -
	                 m.m[0][2] * m.m[1][1] * m.m[2][0];

	float det;
	det =
	    (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
	     m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] - m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
	     m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
	     m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
	     m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
	     m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] + m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	result = result / det;

	return result;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result;
	result = {
	    {
         {
	            m.m[0][0],
	            m.m[1][0],
	            m.m[2][0],
	            m.m[3][0],
	        }, {
	            m.m[0][1],
	            m.m[1][1],
	            m.m[2][1],
	            m.m[3][1],
	        }, {
	            m.m[0][2],
	            m.m[1][2],
	            m.m[2][2],
	            m.m[3][2],
	        }, {
	            m.m[0][3],
	            m.m[1][3],
	            m.m[2][3],
	            m.m[3][3],
	        }, }
    };
	return result;
}

Matrix4x4 MakeIdentity4x4() {
	return {
	    {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}}
    };
}

Matrix4x4 operator/(const Matrix4x4& m, float scalar) {
	return {
	    {{m.m[0][0] / scalar, m.m[0][1] / scalar, m.m[0][2] / scalar, m.m[0][3] / scalar},
	     {m.m[1][0] / scalar, m.m[1][1] / scalar, m.m[1][2] / scalar, m.m[1][3] / scalar},
	     {m.m[2][0] / scalar, m.m[2][1] / scalar, m.m[2][2] / scalar, m.m[2][3] / scalar},
	     {m.m[3][0] / scalar, m.m[3][1] / scalar, m.m[3][2] / scalar, m.m[3][3] / scalar}}
    };
}
