#pragma once

#include "Matrix4x4.h"
#include "Quaternion.h"
#include "Vector3.h"

void ScalarScreenPrintf(int x, int y, float value, const char* label);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);
