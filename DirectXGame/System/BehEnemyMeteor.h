#pragma once

#include "../ConstantVal.h"
#include "../System/Behavior.h"

namespace Atrum {

class CmpReflectableBullet;
class CmpEnemy;
class FrameDeltaTime;

class BehEnemyMeteor : public Behavior {
private:
	FrameDeltaTime* frameDeltaTime_ = nullptr;

	CmpReflectableBullet* cmpBullet_ = nullptr;
	CmpEnemy* cmpEnemy_ = nullptr;

	float shotCoolTimer_ = 0.0f;
	float shotCoolTime_ = 0.25f;

	float bulletShotY_ = kCeilHeight + 1.0f;

	float bulletSpeed_ = 24.0f;

	uint32_t shotCount_ = 0;
	uint32_t shotCountMax_ = 15;

public:
	void ResolveDependence() override;
	void Initialize() override;
	void Execute() override;
};

} // namespace Atrum