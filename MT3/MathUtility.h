#pragma once

#include "Vector3.h"

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);
Vector3 Reflect(const Vector3& input, const Vector3& normal);
