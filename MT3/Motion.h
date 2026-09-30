#pragma once

#include "GeometryTypes.h"
#include "Vector3.h"

void BallSpring(Ball& ball, Spring& spring, float deltaTime = 1.0f / 60.0f);
void CircularMotion(Vector3& position, const Vector3& center, float& angle, float angularVelocity, float radius, float deltaTime = 1.0f / 60.0f);
