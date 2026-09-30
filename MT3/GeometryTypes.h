#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"

struct Triangle {
	Vector3 vertices[3];
};

struct AABB {
	Vector3 min;
	Vector3 max;

	void SwapMinMax();
};

struct OBB {
	Vector3 center;
	Vector3 orientations[3];
	Vector3 size;

	Matrix4x4 GetWorldMatrix() const;
	void UpdateOBBOrientations(const Vector3& rotate);
};

struct Spring {
	Vector3 anchor;           // アンカー
	float naturalLength;      // 自然長
	float stiffness;          // 剛性
	float dampingCoefficient; // 減衰係数
};

struct Ball {
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	float mass;
	float radius;
	unsigned int color;
};

struct Pendulum {
	Vector3 anchor;
	float length;
	float angle;
	float angularVelocity;
	float angularAcceleration;
};

struct ConicalPendulum {
	Vector3 anchor;
	float length;
	float halfApexAngle;
	float angle;
	float angularVelocity;
};
