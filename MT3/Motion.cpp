#include "Motion.h"

#include <cmath>

void BallSpring(Ball& ball, Spring& spring, float deltaTime) {
	const Vector3 diff = ball.position - spring.anchor;
	const float length = Vector3::Length(diff);
	if (length != 0.0f) {
		const Vector3 direction = Vector3::Normalize(diff);
		const Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
		const Vector3 displacement = (ball.position - restPosition) * length;
		const Vector3 restoringForce = displacement * -spring.stiffness;
		const Vector3 dampingForce = ball.velocity * -spring.dampingCoefficient;
		ball.acceleration = (restoringForce + dampingForce) / ball.mass;
	}

	ball.velocity += ball.acceleration * deltaTime;
	ball.position += ball.velocity * deltaTime;
}

void CircularMotion(Vector3& position, const Vector3& center, float& angle, float angularVelocity, float radius, float deltaTime) {
	angle += angularVelocity * deltaTime;
	position.x = center.x + std::cos(angle) * radius;
	position.y = center.y + std::sin(angle) * radius;
	position.z = center.z;
}
