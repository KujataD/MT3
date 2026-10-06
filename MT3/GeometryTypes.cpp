#include "GeometryTypes.h"

#include <algorithm>

void AABB::SwapMinMax() {
	if (min.x > max.x) {
		std::swap(min.x, max.x);
	}
	if (min.y > max.y) {
		std::swap(min.y, max.y);
	}
	if (min.z > max.z) {
		std::swap(min.z, max.z);
	}
}

Matrix4x4 OBB::GetWorldMatrix() const {
	return MakeAffineMatrixOrientations(orientations, center);
}

void OBB::UpdateOBBOrientations(const Vector3& rotate) {
	// 回転行列を生成
	const Matrix4x4 rotateMatrix = MakeRotateZMatrix(rotate.z) * MakeRotateYMatrix(rotate.y) * MakeRotateXMatrix(rotate.x);

	// 回転行列から各軸を抽出して正規化
	orientations[0] = Normalize({rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2]});
	orientations[1] = Normalize({rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2]});
	orientations[2] = Normalize({rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2]});
}
