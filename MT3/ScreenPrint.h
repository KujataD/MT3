#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);
