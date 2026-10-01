#pragma once

#include "../ConstantVal.h"
#include "../System/Behavior.h"

namespace Atrum {

class CmpReflectableBullet;
class CmpEnemy;
class FrameDeltaTime;

class BehEnemyWallShot : public Behavior {
private:
	FrameDeltaTime* frameDeltaTime_ = nullptr;

	CmpReflectableBullet* cmpBullet_ = nullptr;
	CmpEnemy* cmpEnemy_ = nullptr;

	float shotCoolTimer_ = 0.0f;
	float shotCoolTime_ = 0.75f;

	float rapidBulletSpeed_ = 12.0f;
	float slowBulletSpeed_ = 5.0f;

	float bulletShotY_ = kCeilHeight + 0.75f;

	uint32_t shotCount_ = 0;
	uint32_t shotCountMax_ = 2;

public:
	void ResolveDependence() override;
	void Initialize() override;
	void Execute() override;
};

} // namespace Atrum