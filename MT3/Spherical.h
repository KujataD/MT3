#pragma once
#include "Vector3.h"
#include <algorithm>

struct Spherical {
	float radius; // 動径 r 接近後退
	float theta;  // 仰角 θ 見下ろし角度
	float phi;    // 方位角 φ 旋回
};

Vector3 ToCartesian(const Spherical s);

Spherical ToSpherical(const Vector3& p);