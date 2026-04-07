#include <Novice.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "LC1B_07_オオツカ_ダイチ_MT3_00_02";

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
	Matrix4x4 m1 = { 
		3.2f, 0.7f, 9.6f, 4.4f,
		5.5f, 1.3f, 7.8f, 2.1f,
		6.9f, 8.0f, 2.6f, 1.0f,
		0.5f, 7.2f, 5.1f, 3.3f };

	Matrix4x4 m2 = { 
		4.1f, 6.5f, 3.3f, 2.2f,
		8.8f, 0.6f, 9.9f, 7.7f,
		1.1f, 5.5f, 6.6f, 0.0f,
		3.3f, 9.9f, 8.8f, 2.2f };

	Matrix4x4 resultAdd = Add(m1, m2);
	Matrix4x4 resultMultiply = Multiply(m1, m2);
	Matrix4x4 resultSubtract = Subtract(m1, m2);
	Matrix4x4 inverseM1 = Inverse(m1);
	Matrix4x4 inverseM2 = Inverse(m2);
	Matrix4x4 transposeM1 = Transpose(m1);
	Matrix4x4 transposeM2 = Transpose(m2);
	Matrix4x4 identity = MakeIdentity4x4();

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

		MatrixScreenPrintf(0, 0, resultAdd, "Add");
		MatrixScreenPrintf(0, kRowHeight * 5, resultSubtract, "Subtract");
		MatrixScreenPrintf(0, kRowHeight * 5 * 2, resultMultiply, "Multiply");
		MatrixScreenPrintf(0, kRowHeight * 5 * 3, inverseM1, "inverseM1");
		MatrixScreenPrintf(0, kRowHeight * 5 * 4, inverseM2, "inverseM2");
		MatrixScreenPrintf(kColumnWidth * 5, 0, transposeM1, "transposeM1");
		MatrixScreenPrintf(kColumnWidth * 5, kRowHeight * 5, transposeM2, "transposeM2");
		MatrixScreenPrintf(kColumnWidth * 5, kRowHeight * 5 * 2, identity, "identity");

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

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char *label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
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

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) { 	return {
	    {{	m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0],
			m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1],
			m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2], 
			m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3], 
	     },																				 
	     {																				 
			m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0],
			m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1],
			m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2], 
			m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3] 
		 },																				 
		 {																				 
			m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0],
			m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1],
			m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2],
			m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3],
		 },																				 
		 {																				 
			m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0],
			m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1],
			m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2],
			m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3],
		 },
		}
    }; 
}

Matrix4x4 Inverse(const Matrix4x4& m) { 	
	Matrix4x4 result;

	result.m[0][0] = 
		  m.m[1][1] * m.m[2][2] * m.m[3][3] 
		+ m.m[1][2] * m.m[2][3] * m.m[3][1] 
		+ m.m[1][3] * m.m[2][1] * m.m[3][2] 
		- m.m[1][1] * m.m[2][3] * m.m[3][2] 
		- m.m[1][2] * m.m[2][1] * m.m[3][3] 
		- m.m[1][3] * m.m[2][2] * m.m[3][1];

	result.m[0][1] = 
		- m.m[0][1] * m.m[2][2] * m.m[3][3] 
		- m.m[0][2] * m.m[2][3] * m.m[3][1] 
		- m.m[0][3] * m.m[2][1] * m.m[3][2] 
		+ m.m[0][1] * m.m[2][3] * m.m[3][2] 
		+ m.m[0][2] * m.m[2][1] * m.m[3][3] 
		+ m.m[0][3] * m.m[2][2] * m.m[3][1];

	result.m[0][2] = 
		  m.m[0][1] * m.m[1][2] * m.m[3][3] 
		+ m.m[0][2] * m.m[1][3] * m.m[3][1] 
		+ m.m[0][3] * m.m[1][1] * m.m[3][2] 
		- m.m[0][1] * m.m[1][3] * m.m[3][2] 
		- m.m[0][2] * m.m[1][1] * m.m[3][3] 
		- m.m[0][3] * m.m[1][2] * m.m[3][1];

	result.m[0][3] = 
		- m.m[0][1] * m.m[1][2] * m.m[2][3] 
		- m.m[0][2] * m.m[1][3] * m.m[2][1]
		- m.m[0][3] * m.m[1][1] * m.m[2][2]
		+ m.m[0][1] * m.m[1][3] * m.m[2][2]
		+ m.m[0][2] * m.m[1][1] * m.m[2][3] 
		+ m.m[0][3] * m.m[1][2] * m.m[2][1];

	result.m[1][0] = 
		- m.m[1][0] * m.m[2][2] * m.m[3][3] 
		- m.m[1][2] * m.m[2][3] * m.m[3][0] 
		- m.m[1][3] * m.m[2][0] * m.m[3][2] 
		+ m.m[1][0] * m.m[2][3] * m.m[3][2] 
		+ m.m[1][2] * m.m[2][0] * m.m[3][3] 
		+ m.m[1][3] * m.m[2][2] * m.m[3][0];

	result.m[1][1] = 
		  m.m[0][0] * m.m[2][2] * m.m[3][3] 
		+ m.m[0][2] * m.m[2][3] * m.m[3][0] 
		+ m.m[0][3] * m.m[2][0] * m.m[3][2] 
		- m.m[0][0] * m.m[2][3] * m.m[3][2] 
		- m.m[0][2] * m.m[2][0] * m.m[3][3] 
		- m.m[0][3] * m.m[2][2] * m.m[3][0];

	result.m[1][2] = 
		- m.m[0][0] * m.m[1][2] * m.m[3][3]
		- m.m[0][2] * m.m[1][3] * m.m[3][0]
		- m.m[0][3] * m.m[1][0] * m.m[3][2]
		+ m.m[0][0] * m.m[1][3] * m.m[3][2] 
		+ m.m[0][2] * m.m[1][0] * m.m[3][3] 
		+ m.m[0][3] * m.m[1][2] * m.m[3][0];

	result.m[1][3] = 
		  m.m[0][0] * m.m[1][2] * m.m[2][3]
		+ m.m[0][2] * m.m[1][3] * m.m[2][0] 
		+ m.m[0][3] * m.m[1][0] * m.m[2][2]
		- m.m[0][0] * m.m[1][3] * m.m[2][2]
		- m.m[0][2] * m.m[1][0] * m.m[2][3] 
		- m.m[0][3] * m.m[1][2] * m.m[2][0];

	result.m[2][0] = 
		  m.m[1][0] * m.m[2][1] * m.m[3][3] 
		+ m.m[1][1] * m.m[2][3] * m.m[3][0]
		+ m.m[1][3] * m.m[2][0] * m.m[3][1]
		- m.m[1][0] * m.m[2][3] * m.m[3][1] 
		- m.m[1][1] * m.m[2][0] * m.m[3][3]
		- m.m[1][3] * m.m[2][1] * m.m[3][0];

	result.m[2][1] = 
		- m.m[0][0] * m.m[2][1] * m.m[3][3]
		- m.m[0][1] * m.m[2][3] * m.m[3][0]
		- m.m[0][3] * m.m[2][0] * m.m[3][1]
		+ m.m[0][0] * m.m[2][3] * m.m[3][1]
		+ m.m[0][1] * m.m[2][0] * m.m[3][3]
		+ m.m[0][3] * m.m[2][1] * m.m[3][0];

	result.m[2][2] =
		  m.m[0][0] * m.m[1][1] * m.m[3][3] 
		+ m.m[0][1] * m.m[1][3] * m.m[3][0]
		+ m.m[0][3] * m.m[1][0] * m.m[3][1] 
		- m.m[0][0] * m.m[1][3] * m.m[3][1]
		- m.m[0][1] * m.m[1][0] * m.m[3][3]
		- m.m[0][3] * m.m[1][1] * m.m[3][0];

	result.m[2][3] = 
		- m.m[0][0] * m.m[1][1] * m.m[2][3]
		- m.m[0][1] * m.m[1][3] * m.m[2][0]
		- m.m[0][3] * m.m[1][0] * m.m[2][1]
		+ m.m[0][0] * m.m[1][3] * m.m[2][1]
		+ m.m[0][1] * m.m[1][0] * m.m[2][3]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0];

	result.m[3][0] =  
		- m.m[1][0] * m.m[2][1] * m.m[3][2] 
		- m.m[1][1] * m.m[2][2] * m.m[3][0] 
		- m.m[1][2] * m.m[2][0] * m.m[3][1]
		+ m.m[1][0] * m.m[2][2] * m.m[3][1]
		+ m.m[1][1] * m.m[2][0] * m.m[3][2]
		+ m.m[1][2] * m.m[2][1] * m.m[3][0];

	result.m[3][1] = 
		  m.m[0][0] * m.m[2][1] * m.m[3][2]
		+ m.m[0][1] * m.m[2][2] * m.m[3][0] 
		+ m.m[0][2] * m.m[2][0] * m.m[3][1]
		- m.m[0][0] * m.m[2][2] * m.m[3][1]
		- m.m[0][1] * m.m[2][0] * m.m[3][2]
		- m.m[0][2] * m.m[2][1] * m.m[3][0];

	result.m[3][2] = 
		- m.m[0][0] * m.m[1][1] * m.m[3][2] 
		- m.m[0][1] * m.m[1][2] * m.m[3][0] 
		- m.m[0][2] * m.m[1][0] * m.m[3][1] 
		+ m.m[0][0] * m.m[1][2] * m.m[3][1] 
		+ m.m[0][1] * m.m[1][0] * m.m[3][2] 
		+ m.m[0][2] * m.m[1][1] * m.m[3][0];

	result.m[3][3] = 
		  m.m[0][0] * m.m[1][1] * m.m[2][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0]
		+ m.m[0][2] * m.m[1][0] * m.m[2][1] 
		- m.m[0][0] * m.m[1][2] * m.m[2][1]
		- m.m[0][1] * m.m[1][0] * m.m[2][2] 
		- m.m[0][2] * m.m[1][1] * m.m[2][0];

	float det;
	det =
	    ( m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] 
		+ m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] 
		+ m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] 
		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] 
		- m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] 
		- m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] 
		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] 
		- m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] 
		- m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] 
		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] 
		+ m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] 
		+ m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] 
		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] 
		+ m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] 
		+ m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] 
		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] 
		- m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] 
		- m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] 
		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] 
		- m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] 
		- m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] 
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] 
		+ m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] 
		+ m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

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
	        }, 
		}
    };
	return result; 
}

Matrix4x4 MakeIdentity4x4() {
	return {
	    {
		{
			1.0f, 0.0f, 0.0f, 0.0f 
		},
		{
			0.0f, 1.0f, 0.0f, 0.0f
		},
		{
			0.0f, 0.0f, 1.0f, 0.0f
		},
		{
			0.0f, 0.0f, 0.0f, 1.0f
		}
		}
    };
}

Matrix4x4 operator/(const Matrix4x4& m, float scalar) {	
	return {
		{
			{m.m[0][0] / scalar, m.m[0][1] / scalar, m.m[0][2] / scalar, m.m[0][3] / scalar},
			{m.m[1][0] / scalar, m.m[1][1] / scalar, m.m[1][2] / scalar, m.m[1][3] / scalar},
			{m.m[2][0] / scalar, m.m[2][1] / scalar, m.m[2][2] / scalar, m.m[2][3] / scalar},
			{m.m[3][0] / scalar, m.m[3][1] / scalar, m.m[3][2] / scalar, m.m[3][3] / scalar}
		}
	}; }
