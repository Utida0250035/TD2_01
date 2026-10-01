#include "ShieldEnemy.h"
#include <numbers>

void ShieldEnemy::MoveMotion(const float deltaTime) {

	walkMotionTimer_ += deltaTime;

	if (walkMotionTimer_ >= kWalkMotionTime_) {

		walkMotionTimer_ = 0.0f;
	}

	float param = EaseInQuad(std::sin(std::numbers::pi_v<float> * 2.0f * walkMotionTimer_ / kWalkMotionTime_));

	worldTransform_.rotation_.y = kWalkMotionAngleStart_ + kWalkMotionAngleEnd_ * (param + 1.0f) * 0.5f;
}